/*
 * ias_pinout.h
 *
 * Created: 10/12/2015 9:33:00 PM
 *  Author: Sergio
 *
 *  Description: Defines the pinout for all ports 
 */ 



#ifndef IAS_PINOUT_H_
#define IAS_PINOUT_H_
    
    #include <avr/io.h>
    
    /* ============== Communication ============== */

    // 4x4 Keypad { keypad.h }
    #define KEYPADPORT PORTC
    #define KEYPADPIN  PINC
    #define ROW1 3
    #define ROW2 2
    #define ROW3 1
    #define ROW4 0
    #define COL1 7
    #define COL2 6
    #define COL3 5
    #define COL4 4

    // Shift Register { shift_reg.h }
    #define SHIFT_PORT PORTB
    #define SER_PIN   0
    #define RCLK_PIN  1
    #define SRCLK_PIN 2
    #define SRCLR_PIN 3

    /* ============== Sensors ============== */

    // DHT22 Temp/Humidity Sensor { dht22.h }
    #define DHT_DDR DDRA
    #define DHT_PORT PORTA
    #define DHT_PIN PINA
    #define DHT_INPUTPIN 5

    // DS18B20 Temperature Probe { ds18b20.h }
    #define DS18B20_PORT PORTA
    #define DS18B20_DDR  DDRA
    #define DS18B20_PIN  PINA
    #define DS18B20_DQ   6
    
    // PIR Motion Sensor { pir.h }
    #define PIR_DDR  DDRB
    #define PIR_READ PINB
    #define PIR_PIN  6

    /* ============== Display ============== */

    // 16x2 LUMEX LCD { lumex_lcd.h }
    //#define DATA_BUS PORTB          // port connected to pins 7-14 of LCD display | WE NOW USE SHIFT REGISTERS ON DEFINED PINS
    #define CONTROL_BUS PORTB	      // port connected to pins 4 and 6 of LCD disp.
    #define RS 4			          // pin number of uC connected to pin 4 of LCD disp.
    #define E 5			              // pin number of uC connected to pin 6 of LCD disp.

    // NOKIA 84x48 PCD8544 LCD { pcd8544.h }
    #define LCD_PORT            PORTA
    #define LCD_SCE_HI			LCD_PORT |= (1<<4)
    #define LCD_SCE_LO			LCD_PORT &= ~(1<<4)
    #define LCD_RST_HI			LCD_PORT |= (1<<3)
    #define LCD_RST_LO			LCD_PORT &= ~(1<<3)
    #define LCD_DC_HI			LCD_PORT |= (1<<2)
    #define LCD_DC_LO			LCD_PORT &= ~(1<<2)
    #define LCD_DN_HI			LCD_PORT |= (1<<1)
    #define LCD_DN_LO			LCD_PORT &= ~(1<<1)
    #define LCD_SCLK_HI			LCD_PORT |= (1<<0)
    #define LCD_SCLK_LO			LCD_PORT &= ~(1<<0)
    //#define LCD_BACKLIGHT_HI	LCD_PORT |= (1<<5)   // Optional
    //#define LCD_BACKLIGHT_LO	LCD_PORT &= ~(1<<5)  // Optional
    
    #define LCD_C 0 // Command
    #define LCD_D 1 // Data

    #define LCD_WIDTH	84
    #define LCD_HEIGHT	48

#endif /* IAS_PINOUT_H_ */