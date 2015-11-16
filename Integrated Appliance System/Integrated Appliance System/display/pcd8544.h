#ifndef __PCD8544_H__
#define __PCD8544_H__

#include <avr/io.h>
/* NOTE: Macros/port definitions moved to ias_pinout.h! */

void LCDWrite(uint8_t dc, uint8_t data);
void gotoXY(unsigned char x, unsigned char y);
void LCDChar(unsigned char c);
void LCDSprite(unsigned char* str);
void LCDStr(const char* str);
void LCDNum(const char* nums, unsigned char x, unsigned char y, const char append);
void LCDClear(void);
//void LCDBacklight(uint8_t enabled);
void LCDInit(void);
void LCDBitmap(const unsigned char myarray[]);
void LCDWriteMap(unsigned char myarray[], unsigned short map_position);


#endif // __PCD8544_H__