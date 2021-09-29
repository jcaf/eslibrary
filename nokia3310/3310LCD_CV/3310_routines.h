//********************************************************
//****  Header file for 3310_routines.c  *****
//********************************************************
//Controller:	ATmega32 (Clock: 1 Mhz-internal)
//Compiler:		ImageCraft ICCAVR
//Author:		CC Dharmani, Chennai (India)
//Date:			Sep 2008
//********************************************************

#ifndef _3310_ROUTINES_H_

#define _3310_ROUTINES_H_


#define SET_DC_PIN                 PORTB |= 0x01  
#define CLEAR_DC_PIN               PORTB &= ~0x01 
#define SET_SCE_PIN                PORTB |= 0x04
#define CLEAR_SCE_PIN              PORTB &= ~0x04
#define SET_RST_PIN                PORTB |= 0x10
#define CLEAR_RST_PIN              PORTB |= 0x10

 
/*--------------------------------------------------------------------------------------------------
                                 Public function prototypes
--------------------------------------------------------------------------------------------------*/
void LCD_init ( void );
void LCD_clear ( void );
void LCD_gotoXY ( unsigned char x, unsigned char );
void LCD_writeChar ( unsigned char );
void LCD_writeData ( unsigned char );
void LCD_writeCommand ( unsigned char );
void LCD_writeString_F ( flash unsigned char*);
void LCD_drawBorder (void );

#endif  //  _3310_ROUTINES_H_