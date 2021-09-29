//********************************************************
//****  Header file for 3310_routines.c  *****
//********************************************************
//Controller:	ATmega32 (Clock: 1 Mhz-internal)
//Compiler:		AVR_GCC
// es necesario que sus pins inicien en alto
//********************************************************
#ifndef _GLCD3310_H_

#define _GLCD3310_H_

#include <avr/pgmspace.h>

//Kicad definitions
//RST = PB0
#define SET_RST_PIN                PORTB |= 0x01
#define CLEAR_RST_PIN              PORTB |= 0x01

//DC PB2
#define SET_DC_PIN                 PORTB |= 0x04
#define CLEAR_DC_PIN               PORTB &= ~0x04
//CE Chip Enable/select PB1
#define SET_SCE_PIN                PORTB |= 0x02
#define CLEAR_SCE_PIN              PORTB &= ~0x02


 
/*--------------------------------------------------------------------------------------------------
                                 Public function prototypes
--------------------------------------------------------------------------------------------------*/
void LCD_init ( void );
void LCD_clear ( void );
void LCD_update ( void );
void LCD_gotoXY ( unsigned char x, unsigned char y );
void LCD_writeChar ( unsigned char character );
void LCD_writeChar_megaFont ( unsigned char character );
void LCD_writeData ( unsigned char data );
void LCD_writeCommand ( unsigned char command );
void LCD_writeString_megaFont ( char *string );
void LCD_writeString_F ( const char *string);
void LCD_setPixel ( unsigned char x, unsigned char y);
void LCD_clearPixel ( unsigned char x, unsigned char y);
void LCD_drawLine ( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2 );
void LCD_drawBorder (void );
void delay_ms ( int millisec );
void spi_init(void);

#endif  //  _GLCD3310_H_
