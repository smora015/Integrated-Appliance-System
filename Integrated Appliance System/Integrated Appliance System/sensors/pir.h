/*
 * pir.h
 *
 * Created: 12/8/2014 3:25:15 AM
 *  Author: Sergio
 */ 


#ifndef PIR_H_
#define PIR_H_

/* NOTE: Macros/port definitions moved to ias_pinout.h! */
#include "../ias_pinout.h"

unsigned char pir_read()
{
	PIR_DDR &= ~(1 << PIR_PIN); // Set input
	return ((PIR_READ & ( 1 << PIR_PIN) ) >> PIR_PIN); 
}


#endif /* PIR_H_ */