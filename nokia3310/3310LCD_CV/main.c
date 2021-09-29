//********************************************************
//***** PROGRAM FOR Interfacing NOKIA 3310 Display ******
//********************************************************
//Controller:	ATmega32 (Clock: 1 Mhz-internal)
//Compiler:		CodeVision AVR
//Author:		CC Dharmani, Chennai (India)
//Date:			Sep 2008
//********************************************************

#include <mega8.h>
#include <delay.h>
#include "3310_routines.h"

void port_init(void)
{
 PORTB = 0xFF;
 DDRB  = 0xFF;
 PORTC = 0x00; 
 DDRC  = 0x00;
 PORTD = 0x00;
 DDRD  = 0xFF;
}

//SPI initialize
//clock rate: 250000hz
void spi_init(void)
{
 SPCR = 0x58; //setup SPI
}

//TWI initialize
// bit rate:2 (freq: 50Khz)
void twi_init(void)
{
 TWCR= 0X00; //disable twi
 TWBR= 0x02; //set bit rate
 TWSR= 0x00; //set prescale
 TWAR= 0x00; //set slave address
 TWCR= 0x44; //enable twi
}

//call this routine to initialize all peripherals
void init_devices(void)
{
 port_init();
 spi_init();
 twi_init();
 
 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x00; //timer interrupt sources
}

//
void main(void)
{
 unsigned char LED=0;
 
 init_devices();

 LCD_init();
 
 delay_ms(100);	   
 
 LCD_drawBorder();
 
 LCD_gotoXY (4,2);
 LCD_writeString_F("3310_LCD");
 
 LCD_gotoXY (4,3);
 LCD_writeString_F("by DHARMANI");
 

 while(1)
 {
   if(LED == 0) 
   {
 	 PORTD |= 0x01;
	 LED = 1;
   }
   else
   {
	 PORTD &= ~0x01;
	 LED = 0;
   } 
   
 }  
   
}