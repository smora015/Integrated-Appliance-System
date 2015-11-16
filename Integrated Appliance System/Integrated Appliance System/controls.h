/*
 * controls.h
 *
 * Created: 12/15/2014 7:02:03 PM
 *  Author: Sergio
 */ 


#ifndef CONTROLS_H_
#define CONTROLS_H_

// CONTROL PINS
#define MOTOR_PORT     PORTA
#define FAN_PIN        
// USART Signals
#define SEND_REPORT     0xAA
#define ENABLE_FAND     0x44
#define DISABLE_FAND    0x33
#define ENABLE_MOTORH   0x22
#define DISABLE_MOTORH  0x11

#define THREE_SIXTY  64*(360/5.625)

// Global Variables
unsigned char usart_byte;
unsigned char bluetooth_byte;
char* report;

void USART_send_string( const char* string );
void Control_Tick();

#endif /* CONTROLS_H_ */