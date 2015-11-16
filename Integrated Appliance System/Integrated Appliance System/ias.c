/*
 * ias.c
 *
 * Created: 12/15/2014 5:46:22 AM
 *  Author: Sergio
 */ 
#include "ias.h"
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "display/lumex_lcd.h"        // 16x2 LCD
#include "display/pcd8544.h"          // 84x48 LCD
#include "sensors/pir.h"              // Motion Sensor
#include "sensors/ds18b20.h"          // Temperature Probe
#include "sensors/dht.h"		      // Temperature Sensor
#include "com/keypad.h"               // Keypad
#include "com/usart_ATmega1284.h"     // USART

// State Machine variables
enum menu_states {init, intro, options, stats, send, control, settings} menu_state; // States for IAS_MenuTick()

short screen_counter = 0;		    // Used to count number of seconds a state will be on for	
char buffer[3];						// Holds temperature and humidity sensor values

void USART_send_stringb( const char* string )
{
	int index = 0;
	while( string[index] != '\0')
	{
		USART_Send(string[index], 0);
		++index;
	}
}

void IAS_init()
{			
	// Initialize USART communication (Bluetooth)
	initUSART(0);
	USART_Flush(0);

	// Initialize 16x2 LCD
	LCD_init();

	// Initialize Nokia LCD
	LCDInit();
	LCDClear();

	// Initialize all variables
	humidity = (float*) malloc(sizeof(float));
	temperature = (float*) malloc(sizeof(float));
	wh_temperature = (float*) malloc(sizeof(float));
	menu_cursor = 0;
	key_press = 0;
	
	temp_toggle = DEGREES_F;
	ias_mode =	DAY_MODE;
	motor_pid = MOTOR_PID;
	
	space_heater = OFF;
	water_heater = OFF;
	closet_dryer = OFF;

	ias_intro_screen(); // Display intro screen
	
	return;
}

void IAS_MenuTick()
{
	// Implemented as Mealy (actions in transitions exist)
	switch( menu_state ) // Transitions/Actions 
	{
		case init:
			IAS_init();
			menu_state = intro; // TRANSITION: to first state
			break; // END state init
		
		case intro:
			// Display intro screen for 3 seconds
			if( ++screen_counter > 3000)
			{
				menu_state = options; // TRANSITION: Go to options
				LCDClear();			  // Clear Nokia screen
				LCD_ClearScreen();    // Clear 16x2 screen
				ias_options_screen(); // Display options
				screen_counter = 0;   // Reset timing counter
			}
			break; // END state intro
		
		case options:
			// Display cursor to select an option. If user presses enter, move to:
			if( ias_get_key_press_up() )
			{
				if( key_press == PRESS_UP || key_press == PRESS_DOWN )
					ias_update_cursor( key_press, 1, 4 );
					
				else if( key_press == PRESS_ENTER )
				{
					// Go to specific states
					if( menu_cursor == OPTION_CONTROLS )
					{
						menu_state = control; // TRANSITION: controls screen
						LCDClear();			  // Clear Nokia screen
						ias_controls_screen();
					}
					else if( menu_cursor == OPTION_SEND )
					{
						menu_state = send;    // TRANSITION: send data to phone screen
						LCDClear();			  // Clear Nokia screen
						ias_send_screen();
					}
					else if( menu_cursor == OPTION_STATS )
					{
						menu_state = stats;   // TRANSITION: stats screen
						LCDClear();			  // Clear Nokia screen
						ias_stats_screen();
					}
					else if( menu_cursor == OPTION_SETTINGS )
					{
						menu_state = settings; // TRANSITION: settings screen
						LCDClear();			   // Clear Nokia screen
						ias_settings_screen();
					}
				}
			
				key_press = 0;
			}
			break; // END state options
		
		case stats:
			// Send user back to options if they press '1'
			if( ias_get_key_press_up() )
			{
				if( key_press == PRESS_BACK )
				{
					menu_state = options;
					LCDClear();			  // Clear Nokia screen
					ias_options_screen();
				}
				key_press = 0;
			}
			
			// Refresh temperature data every 7 seconds
			if( ++screen_counter > 7000)
			{
				LCDClear();
				ias_stats_screen();
				screen_counter = 0;
			}
			break; // END state stats
		
		case send:
			// Send user back to options if they press '1'
			if( ias_get_key_press_up() )
			{
				if( key_press == PRESS_ENTER )
				{					
					// Inform user that data was sent for 4 seconds
					gotoXY(6*4,3);
					LCDClear();
					LCDStr(PSTR("Sent!    Please wait."));
					
					// Signal report sending to microcontroller 2
					USART_Send(SEND_REPORT, 0);
					
					char* a = "\n\nIAS - Integrated Appliance System\n====================";
					char* b = "\nCD Temperature: ";
					char c[3];
					itoa((int)(*temperature*9/5 + 32), c, 10);
					char* d = "\nWH Temperature: ";
					char e[3];
					itoa((int)(*wh_temperature*9/5 + 32), e, 10);
					char* f = "\nHumidity: ";
					char g[2];
					itoa((int)(*humidity), g, 10);
					
				
					USART_send_stringb(a);
					USART_send_stringb(b);
					USART_send_stringb(c);
					USART_send_stringb(d);
					USART_send_stringb(e);
					USART_send_stringb(f);
					USART_send_stringb(g);
					USART_Send(0xFF, 0);   // Signal end of transmission
					//_delay_ms(4000);
			
					// Go back to menu
					LCDClear();
					ias_options_screen();
					menu_state = options;
				}
				else if( key_press == PRESS_BACK )
				{
					menu_state = options;
					LCDClear();			  // Clear Nokia screen
					ias_options_screen();
				}
				key_press = 0;
			}
			
			break; // END state send
		
		case control:
			// Send user back to options if they press '1'
			if( ias_get_key_press_up() )
			{
				if( key_press == PRESS_UP || key_press == PRESS_DOWN)
					ias_update_cursor(key_press, 1, 3);
				else if( key_press == PRESS_BACK )
				{
					menu_state = options;
					LCDClear();			  // Clear Nokia screen
					ias_options_screen();
				}
				else if( key_press == PRESS_ENTER )
				{
					if( menu_cursor == 1 )
					{
						water_heater = (water_heater == ON ? OFF : ON);
					}
					else if( menu_cursor == 2)
					{
						if( closet_dryer == ON)
						{
							// Toggle off and notify other uC
							closet_dryer = OFF;
							//USART_Flush(0);
							USART_Send(DISABLE_FAND, 0);
						}
						else
						{
							// Toggle on and notify other uC
							closet_dryer = ON;
							//USART_Flush(0);
							USART_Send(ENABLE_FAND, 0);
						}

					}
					else if( menu_cursor == 3)
					{
						if( space_heater == ON)
						{
							// Toggle off and notify other uC
							space_heater = OFF;
							//USART_Flush(0);
							USART_Send(DISABLE_MOTORH, 0);
						}
						else
						{
							// Toggle on and notify other uC
							space_heater = ON;
							//USART_Flush(0);
							USART_Send(ENABLE_MOTORH, 0);
						}

					
					}
					// Refresh screen
					LCDClear();
					ias_controls_screen();
				}
				key_press = 0;
			}
			break; // END state control
		
		case settings:
			// Send user back to options if they press '1'
			if( ias_get_key_press_up() )
			{
				if( key_press == PRESS_UP || key_press == PRESS_DOWN )
					ias_update_cursor(key_press, 1, 3);
					
				else if( key_press == PRESS_BACK )
				{
					menu_state = options;
					LCDClear();			  // Clear Nokia screen
					ias_options_screen();
				}
				else if( key_press == PRESS_ENTER )
				{
					if( menu_cursor == 1 )
						temp_toggle = (temp_toggle == DEGREES_C ? DEGREES_F : DEGREES_C);
					else if( menu_cursor == 2)
						ias_mode = (ias_mode == NIGHT_MODE ? DAY_MODE : NIGHT_MODE);
					else if( menu_cursor == 3)
						motor_pid = (motor_pid == MOTOR_MANUAL ? MOTOR_PID : MOTOR_MANUAL);
					
					// Refresh screen
					LCDClear();
					ias_settings_screen();
				}
				key_press = 0;
			}
			break; // END state settings
			
		default:
			menu_state = init;
			break;
	}
	
	return;
}


unsigned char ias_get_key_press_up()
{
	// If user is holding down key...
	if( GetKeypadKey() > '\0')
	{
		// Update our key press variable, but we're not ready yet.
		key_press = GetKeypadKey();
		return 0; // Not ready
	}
	// If user let go of that key...
	else if( GetKeypadKey() == '\0' && key_press > '\0')
	{
		// We're ready, AKA user did a valid press
		return 1;
	}
	
	return 0;
}

void ias_update_cursor(unsigned char direction, unsigned char min, unsigned char max)
{
	if( direction != PRESS_UP && direction != PRESS_DOWN)
	return;

	if( direction == PRESS_DOWN)
	{
		if( (menu_cursor + 1) > max )
		return;

		// Erase old cursor position
		gotoXY( 0, menu_cursor );
		LCDStr( PSTR(" ") );
		
		gotoXY( 0, ++menu_cursor );
	}
	else if( direction == PRESS_UP )
	{
		if( (menu_cursor - 1) < min )
		return;

		// Erase old cursor position
		gotoXY( 0, menu_cursor );
		LCDStr( PSTR(" ") );
		
		gotoXY( 0, --menu_cursor );
	}
	
	// Draw new position and update cursor
	LCDStr( PSTR(">") );
	
	return;
}

void ias_intro_screen()
{
	// Nokia LCD
	gotoXY(0,0);
	LCDStr(PSTR("IAS         Integrated  Appliance   System"));
	gotoXY(0,0);
	
	// 16x2 LCD
	LCD_DisplayString(1, (const unsigned char*)"Welcome to IAS!", 1);
	
	return;
}

void ias_options_screen()
{
	// Nokia LCD
	gotoXY(0,0);
	LCDStr(PSTR(" == Menu =="));
	gotoXY(0,1);
	LCDStr(PSTR(" IAS STATS"));
	gotoXY(0,2);
	LCDStr(PSTR(" SEND DATA-BT"));
	gotoXY(0,3);
	LCDStr(PSTR(" CONTROLS"));
	gotoXY(0,4);
	LCDStr(PSTR(" SETTINGS"));

	// Cursor
	menu_cursor = 1;
	gotoXY(0, menu_cursor);
	LCDStr(PSTR(">"));
	// 16x2 LCD
	LCD_DisplayString(1, (const unsigned char*)"Select an optionB,5,3,9=DIR *=GO", 1);

}

void ias_stats_screen()
{
	// Nokia LCD
	gotoXY(0,0);
	LCDStr(PSTR("==IAS Stats=="));
	gotoXY(0,1);
	LCDStr(PSTR("Dryer T: "));
	
	gotoXY(0,2);
	LCDStr(PSTR("Dryer H: "));
	
	unsigned char parity = dht_gettemperaturehumidity( temperature, humidity);
	if( parity == 0)
	{
		// Dryer temperature
		if( temp_toggle == DEGREES_F)
		{
			itoa((int)(*temperature*9/5 + 32), buffer, 10); // Nokia LCD Temperature
			LCDNum(buffer, 60, 1, 'F');
		}
		else if( temp_toggle == DEGREES_C )
		{
			itoa((int)(*temperature), buffer, 10); // Nokia LCD Temperature
			LCDNum(buffer, 60, 1, 'C');
		}
		
		// Dryer Humidity			
		itoa((int)*humidity, buffer, 10);			    // Nokia LCD Humidity
		LCDNum(buffer, 60, 2, '%');
	}
	else
	{
		gotoXY(60, 1);
		LCDStr(PSTR(" - "));
		gotoXY(60, 2);
		LCDStr(PSTR(" - "));
	}

	// DS18B20 Temperature Sensor
	gotoXY(0,3);
	LCDStr(PSTR("Water T: "));	
	*wh_temperature = ds18b20_gettemp();
	if( temp_toggle == DEGREES_F)
	{
		itoa((int)(*wh_temperature*9/5 + 32), buffer, 10); // Nokia LCD Temperature
		LCDNum(buffer, 60, 3, 'F');
	}
	else if( temp_toggle == DEGREES_C )
	{
		itoa((int)(*wh_temperature), buffer, 10); // Nokia LCD Temperature
		LCDNum(buffer, 60, 3, 'C');
	}
	
	// PIR (Motion) Sensor
	gotoXY(0,4);
	LCDStr(PSTR("Status: "));
	gotoXY(54,4);
	unsigned char motion = pir_read();
	if( motion == 1)
		LCDStr(PSTR("ERR!"));
	else
		LCDStr(PSTR("OK!"));
	
	// 16x2 LCD
	LCD_DisplayString(1, (const unsigned char*)"IAS Statistics  Appliance Status", 1);

}


void ias_send_screen()
{
	// Nokia LCD
	gotoXY(0,0);
	LCDStr(PSTR("EnergyReport"));

	gotoXY(0,1);
	LCDStr(PSTR("============Send? Energyinfo. will  be sent to  mobile."));
	
	// 16x2 LCD
	LCD_DisplayString(1, (const unsigned char*)"IAS Send Data-BT Send to phone", 1);

}

void ias_controls_screen()
{
	// Nokia LCD
	gotoXY(0,0);
	LCDStr(PSTR("=IAS CONTROL="));

	// Water Heater
	gotoXY(0, 1);
	if( water_heater == ON)
		LCDStr(PSTR(" WHeater: ON"));
	else if( water_heater == OFF)
		LCDStr(PSTR(" WHeater:OFF"));
	
	
	// Clothes Dryer
	gotoXY(0,2);
	if( closet_dryer == ON )
		LCDStr(PSTR(" Dryer: ON"));
	else if( closet_dryer == OFF )
		LCDStr(PSTR(" Dryer: OFF"));
	
	// Space Heater
	gotoXY(0,3);
	if( space_heater == ON ) 
		LCDStr(PSTR(" Heater: ON"));
	else if( space_heater == OFF )
		LCDStr(PSTR(" Heater: OFF"));
		
	// Cursor
	menu_cursor = 1;
	gotoXY(0, menu_cursor);	
	LCDStr(PSTR(">"));
	
	// 16x2 LCD
	LCD_DisplayString(1, (const unsigned char*)"Control a component in the IAS", 1);

}

void ias_settings_screen()
{
	// Nokia LCD
	gotoXY(0,0);
	LCDStr(PSTR("IAS SETTINGS"));

	// Cursor
	menu_cursor = 1;
	
	// Temperature Scale
	gotoXY(0, 1);
	LCDStr(PSTR(" TEMP MODE: "));
	
	gotoXY(78, 1);
	if( temp_toggle == DEGREES_F )
		LCDStr(PSTR("F"));
	else if( temp_toggle == DEGREES_C )
		LCDStr(PSTR("C"));
	
	// IAS Mode
	gotoXY(0, 2);
	LCDStr(PSTR(" IAS MODE:  "));
	
	gotoXY(66, 2);
	if( ias_mode == DAY_MODE )
		LCDStr(PSTR("DY"));
	else if( ias_mode == NIGHT_MODE )
		LCDStr(PSTR("NT"));
		
	// Motor Mode
	gotoXY(0, 3);
	LCDStr(PSTR(" MOTOR CTRL:  "));
	
	gotoXY(78,3);
	if( motor_pid == MOTOR_PID )
		LCDStr(PSTR("A"));
	else if( motor_pid == MOTOR_MANUAL )
		LCDStr(PSTR("M"));
		
	// 16x2 LCD
	LCD_DisplayString(1, (const unsigned char*)"Modify settings in the IAS", 1);

	gotoXY(0, menu_cursor);
	LCDStr(PSTR(">"));
}