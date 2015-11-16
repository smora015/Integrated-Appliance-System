/*
 * ias.h
 *
 * Created: 12/15/2014 5:47:08 AM
 *  Author: Sergio
 */ 


#ifndef IAS_H_
#define IAS_H_

// Keypad Presses for Menu
#define PRESS_UP    'B'
#define PRESS_DOWN  '5'
#define PRESS_LEFT  '3'
#define PRESS_RIGHT '9'
#define PRESS_ENTER '*'
#define PRESS_BACK  '1'

// IAS Menu Selection
#define OPTION_STATS    1
#define OPTION_SEND     2
#define OPTION_CONTROLS 3
#define OPTION_SETTINGS 4

// IAS Setting Toggles
#define DEGREES_F		0
#define DEGREES_C		1
#define NIGHT_MODE      0
#define DAY_MODE        1
#define MOTOR_MANUAL    0
#define MOTOR_PID       1
#define ON				1
#define OFF				0

// USART Signals
#define SEND_REPORT     0xAA
#define ENABLE_FAND     0x44
#define DISABLE_FAND    0x33
#define ENABLE_MOTORH   0x22
#define DISABLE_MOTORH  0x11

// Global Variables
float *humidity;			// Holds humidity data for HDT22 (Clothes Dryer)
float *temperature;			// Holds temperature data for DHT22 (Clothes Dryer)
float *wh_temperature;		// Holds temperature data for DS18b20 (Water Heater)
unsigned char menu_cursor;	// Decides what option you're highlighting in the menu
unsigned char key_press;    // The last key to be pressed

// IAS Settings Variables
unsigned char temp_toggle;  // Change temperature scale (Fahrenheit or Celsius)
unsigned char ias_mode;     // IAS can be set in night or day mode.
unsigned char motor_pid;    // Toggle auto or manual motor control

unsigned char space_heater; // Toggle space heater on or off
unsigned char water_heater; // Toggle water heater on or off
unsigned char closet_dryer; // Toggle closet dryer on or off

// Menu Functions
void ias_intro_screen();		                 // Display IAS Intro Message
void ias_options_screen();                       // Display IAS Options
void ias_stats_screen();						 // Display sensor information
void ias_send_screen();							 // Prompt Bluetooth Energy Report
void ias_controls_screen();						 // Display component control
void ias_settings_screen();						 // Display settings to change
void ias_update_cursor(unsigned char direction, unsigned char min, unsigned char max); // Sets cursor to new position
unsigned char ias_get_key_press_up();			 // Returns 1 if key_press gets valid press-release button. Make sure to clear key_press after.

void IAS_MenuTick();							 // Main Task



#endif /* IAS_H_ */