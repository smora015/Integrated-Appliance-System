#ifndef __LUMEX_LCD_H__
#define __LUMEX_LCD_H__

/* NOTE: Macros/port definitions moved to ias_pinout.h! */

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))


void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char Command);
void LCD_WriteData(unsigned char Data);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,const unsigned char *string, unsigned char clear);

#endif // __LUMEX_LCD_H__