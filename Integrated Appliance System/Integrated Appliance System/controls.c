/*
 * controls.c
 *
 * Created: 12/15/2014 7:00:38 PM
 *  Author: Sergio
 */ 
#include "controls.h"
#include "com/usart_ATmega1284.h"
#include <avr/io.h>
#include <stdlib.h>

unsigned char step_count = 0x00;
unsigned char  steps[8] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };	// Clockwise phases
unsigned char bsteps[8] = { 0x09, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01  };  // Counter-Clockwise phases
int count = 0;																	// Counter for number of phases

char temp;
char message[100];

void USART_send_string( const char* string )
{
	USART_Flush(1);
	USART_Flush(0);
	int index = 0;
	while( string[index] )
	{
		USART_Send(string[index], 1);
		++index;
	}
}

enum control_states { init, receive, actuate_gears, disable_gears, actuate_fans, disable_fans, send_report } control_state;
void Control_Tick()
{
	// Mealy FSM (actions and transitions)
	switch( control_state )
	{
		case init:
			report = (char*) malloc(sizeof(char));
			control_state = receive;
			break;
			
		case receive:
			if( USART_HasReceived(0) )
				usart_byte = USART_Receive(0);
			else
				usart_byte = 0x00;
				
			if( USART_HasReceived(1) )
				bluetooth_byte = USART_Receive(1);
			else
				bluetooth_byte = 0x00;
				
			PORTC = usart_byte;
	
			if(usart_byte == ENABLE_MOTORH || bluetooth_byte == ENABLE_MOTORH)
			{
				count = 0;
				control_state = actuate_gears; // TRANSITION: actuate_gears;
			}
			else if(usart_byte == DISABLE_MOTORH || bluetooth_byte == DISABLE_MOTORH)
			{
				count = 0;
				control_state = disable_gears; // TRANSITION: disable_gears;
			}
			else if(usart_byte == ENABLE_FAND || bluetooth_byte == ENABLE_FAND)
			{
				control_state = actuate_fans;  // TRANSITION: actuate_fans
			}
			else if(usart_byte == DISABLE_FAND || bluetooth_byte == DISABLE_FAND)
			{
				control_state = disable_fans;  // TRANSITION: disable_fans
			}
			else if( usart_byte == SEND_REPORT)
			{
				control_state = send_report;
			}
			break;

		case actuate_gears:
			if( count < THREE_SIXTY) // Turn gears 180 degrees
			{
				PORTA = steps[ (step_count++ % 8) ];
				++count;			
			}
			else
			{
				count = 0;
				control_state = receive; // TRANSITION: receive
			}
			break;
		
		case disable_gears:
			if( count < THREE_SIXTY) // Turn gears 180 degrees
			{
				PORTA = bsteps[ (step_count++ % 8) ];
				++count;
			}
			else
			{
				count = 0;
				control_state = receive; // TRANSITION: receive
			}
			break;

		case actuate_fans:
			PORTB = 0x01;
			control_state = receive;
			break;
		
		case disable_fans:
			PORTB = 0x00;
			control_state = receive;
			break;
			
		case send_report:
			count = 0;
			do 
			{
				temp = USART_Receive(0);
				message[count++] = temp;
			} while (temp != 0xFF);
			
			USART_send_string(message);
			count = 0;
			break;
		default:
			control_state = init;
			break;
	}
}