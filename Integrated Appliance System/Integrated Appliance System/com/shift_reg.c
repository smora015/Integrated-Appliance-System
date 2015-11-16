/*
 * shift_reg.c
 *
 * Created: 12/10/2014 12:09:07 PM
 *  Author: Sergio
 */ 
#include "shift_reg.h"
#include "../ias_pinout.h"

void transmit_data(unsigned char data) {
	int i;
	for (i = 7; i >= 0; --i)
	{
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		SHIFT_PORT = (SHIFT_PORT & 0xF0) | (1 << SRCLR_PIN);

		// set SER = next bit of data to be sent.
		SHIFT_PORT |= ((data >> i) & (1 << SER_PIN));
		
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		SHIFT_PORT |= (1 << SRCLK_PIN);
	}
	
	// set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
	SHIFT_PORT |= (1 << RCLK_PIN);
	
	// clears all lines in preparation of a new transmission
	SHIFT_PORT &= (0xF0);
}