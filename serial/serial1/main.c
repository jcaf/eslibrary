/*
 * serial1.c
 *
 * Created: 12/12/2017 12:06:01 p.m.
 *  Author: FIRWAR
 
 http://www.engbedded.com/fusecalc/
 
 fuses:
 -U lfuse:w:0xbf:m -U hfuse:w:0x07:m
 
 program memory:
 avrdude -c usbasp -p m32 -B10 -u -U flash:w:atmelstudio7.hex
 */ 

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
#include "system.h"
#include "serial/serial.h"
#include "usart/usart.h"

uint8_t buff_out[SCIRBUF_BUFF_SIZE];
int main(void)
{
	uint8_t i;
	uint8_t bytes_available;
	USART_Init ( MYUBRR );
	sei();
	
	PORTA = 0x00;
    DDRA = 0xFF;

	while(1)
    {
		bytes_available = scirbuf_bytes_available();
		if (bytes_available>0)
		{
			scirbuf_read_nbytes(buff_out, bytes_available);
			USART_Transmit(bytes_available+0x30);
			for (i = 0; i<bytes_available; i++ )
				{USART_Transmit(buff_out[i]);}
			USART_Transmit('\n');
		}
		USART_Transmit('+');USART_Transmit('-');USART_Transmit('\n');
		delay_ms(3000);
    }
	return 0;
}
