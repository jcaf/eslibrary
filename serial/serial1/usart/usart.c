/*
 * usart.c
 *
 * Created: 12/30/2016 11:21:41 PM
 *  Author: jcaf
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart/usart.h"
#include "serial/serial.h"

void USART_Init( unsigned int ubrr)
{
    /*Set baud rate */
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;
    /*Enable receiver and transmitter */
    UCSRB = (1<<RXEN)|(1<<TXEN) | (1<<RXCIE);//Enable interrupt;
    /* Set frame format: 8data, 1stop bit */
    //UCSRC = (0<<USBS)|(3<<UCSZ0);
    UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);//URSEL necesario para los atmega q comparten
}
void USART_Transmit( unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !( UCSRA & (1<<UDRE)) )
        ;
    /* Put data into buffer, sends the data */
    UDR = data;
}
unsigned char USART_Receive( void )
{
    /* Wait for data to be received */
    while ( !(UCSRA & (1<<RXC)) )
        ;
    /* Get and return received data from buffer */
    return UDR;
}
void USART_Flush( void )
{
    unsigned char dummy;
    while ( UCSRA & (1<<RXC) ) dummy = UDR;
}

ISR(USART_RXC_vect)//ATmega16, ATmega32, ATmega323, ATmega8 
{
	rx_handler();
    //uint8_t  _udr_rx = UDR;
	//USART_Transmit(_udr_rx );
	//PORTA = 0x01;
    //uint8_t u = UDR;
}
