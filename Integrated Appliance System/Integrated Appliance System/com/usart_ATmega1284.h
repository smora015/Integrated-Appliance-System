
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef USART_1284_H
#define USART_1284_H

// USART Setup Values
#define F_CPU 8000000UL // Assume uC operates at 8MHz
#define BAUD_RATE 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) - 1)

////////////////////////////////////////////////////////////////////////////////
//Functionality - Initializes TX and RX on PORT D
//Parameter: usartNum specifies which USART is being initialized
//			 If usartNum != 1, default to USART0
//Returns: None
void initUSART(unsigned char usartNum);
////////////////////////////////////////////////////////////////////////////////
//Functionality - checks if USART is ready to send
//Parameter: usartNum specifies which USART is checked
//Returns: 1 if true else 0
unsigned char USART_IsSendReady(unsigned char usartNum);
////////////////////////////////////////////////////////////////////////////////
//Functionality - checks if USART has successfully transmitted data
//Parameter: usartNum specifies which USART is being checked
//Returns: 1 if true else 0
unsigned char USART_HasTransmitted(unsigned char usartNum);
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - checks if USART has recieved data
//Parameter: usartNum specifies which USART is checked
//Returns: 1 if true else 0
unsigned char USART_HasReceived(unsigned char usartNum);
////////////////////////////////////////////////////////////////////////////////
//Functionality - Flushes the data register
//Parameter: usartNum specifies which USART is flushed
//Returns: None
void USART_Flush(unsigned char usartNum);
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - Sends an 8-bit char value
//Parameter: Takes a single unsigned char value
//			 usartNum specifies which USART will send the char
//Returns: None
void USART_Send(unsigned char sendMe, unsigned char usartNum);
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - receives an 8-bit char value
//Parameter: usartNum specifies which USART is waiting to receive data
//Returns: Unsigned char data from the receive buffer
unsigned char USART_Receive(unsigned char usartNum);

#endif //USART_1284_H