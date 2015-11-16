#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 

// AVR Include Files
#define F_CPU 8000000UL     // 8Mhz Clock
#include <util/delay.h>
#include <avr/io.h>         
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 

// IAS macro and pinout definitions
#include "ias_pinout.h"
//#include "display/lumex_lcd.h"

int main(void) 
{

    //LCD_init();

	for(;;)
	{
		//LCD_DisplayString(0, (const unsigned char*) "Hello World!", 1);
        
        _delay_ms(1000);       // Delay logic for 100 ms
		
		PORTD ^= 0xFF; // UI PORTD bits
			
	}
		
	return 0; 
}


//// FreeRTOS Include Files
//#include "FreeRTOS.h"
//#include "task.h"
//#include "croutine.h"
//#include "usart_ATmega1284.h"

//void ControlTask()
//{
//for( ;; )
//{
//Control_Tick();
//vTaskDelay(1);
//}
//}
//
//void StartSecPulse(unsigned portBASE_TYPE Priority)
//{
//xTaskCreate(ControlTask, (signed portCHAR *)"ControlTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
//}

	//#ifdef main2
	//DDRB = 0xFF; PORTB = 0x00; // Fans (0)
	//DDRC = 0xFF; PORTC = 0x00; // LEDS
	//DDRA = 0xFF; PORTA = 0x00; // Motor
	//initUSART(1);			   // Bluetooth
	//initUSART(0);			   // IAS
	//USART_Flush(0);
	//USART_Flush(1);
	//
	//// Start Tasks
	//StartSecPulse(1);
	//
	//// Run Scheduler
	//vTaskStartScheduler();
	//#else
	//DDRA = 0xFF; PORTA = 0x00; // Nokia LCD: 0-4. DHT22: 5. DS18B20: 6.
	//DDRB = 0xFF; PORTB = 0x00; // 16x2 LCD on Shift Registers
	//DDRC = 0xF0; PORTC = 0x0F; // Keypad
	//DDRD = 0xFF; PORTD = 0x00; // USART interface
	//
	//while(1)
	//{
	//
	//IAS_MenuTick();
	//_delay_ms(1);
	//}
	//#endif