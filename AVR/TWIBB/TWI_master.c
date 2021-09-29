#include "../types.h"
#include "../system.h"
#include "TWI_master.h"


/*! \brief initialize twi master mode
 */ 
void twi_init()
{
	DDR_SCL |= (1 << SCL);
	DDR_SDA |= (1 << SDA);

	write_sda(1);
        write_scl(1);
	
} 

/*! \brief disables twi master mode
 */
void twi_disable()
{
	DDR_SCL &= ~(1 << SCL);
	DDR_SDA &= ~(1 << SDA);

}

/*! \brief Sends start condition
 */
char twi_start_cond(void)
{
        write_sda(0);
	__delay_cycles(DELAY);
	
	write_scl(0);	
	__delay_cycles(DELAY);
	return 1;
    
}

/*! \brief Sends slave address
 */
char send_slave_address(unsigned char read)
{
	return i2c_write_byte(SLAVE_ADDRESS | read );
} 
 
/*! \brief Writes data from buffer.
    \param indata Pointer to data buffer
    \param bytes  Number of bytes to transfer
    \return 1 if successful, otherwise 0
 */

char write_data(unsigned char* indata, char bytes)
{
	unsigned char index, ack = 0;
	
	if(!twi_start_cond())
		return 0;
	if(!send_slave_address(WRITE))
		return 0;	
	
	for(index = 0; index < bytes; index++)
	{
		 ack = i2c_write_byte(indata[index]);
		 if(!ack)
			break;		
	}
	//put stop here
	write_scl(1);
	//__delay_cycles(SCL_SDA_DELAY);
	__delay_us(2);//100Khz
	write_sda(1);
	return ack;
	
}


/*! \brief Writes a byte on TWI.
    \param byte Data 
    \return 1 if successful, otherwise 0
 */
char i2c_write_byte(unsigned char byte)
{
        char bit;
	for (bit = 0; bit < 8; bit++) 
	{
            write_sda((byte & 0x80) != 0);
            toggle_scl();//goes high
            __delay_cycles(DELAY);
            toggle_scl();//goes low
            byte <<= 1;
            __delay_cycles(DELAY);
        }
	//release SDA
	SET_SDA_IN();
	toggle_scl(); //goes high for the 9th clock
	//Check for acknowledgment
	if(READ_SDA())
	{
		return 0;			
	}
	__delay_cycles(DELAY);
	//Pull SCL low
	toggle_scl(); //end of byte with acknowledgment. 
	//take SDA
	SET_SDA_OUT();
	__delay_cycles(DELAY); 
	return 1;
		
}	
/*! \brief Reads data into buffer.
    \param data Pointer to data buffer
    \param bytes  Number of bytes to read
    \return 1 if successful, otherwise 0
 */
char read_bytes(unsigned char* data, char bytes)
{
	unsigned char index,success = 0;
	if(!twi_start_cond())
		return 0;
	if(!send_slave_address(READ))
		return 0;	
	for(index = 0; index < bytes; index++)
	{
		success = i2c_read_byte(data, bytes, index);
		if(!success)
			break; 
	}
	//put stop here
	write_scl(1);
	//__delay_cycles(SCL_SDA_DELAY);
	__delay_us(2);//100Khz
	write_sda(1);
	return success;
	
	
}	

/*! \brief Reads one byte into buffer.
    \param rcvdata Pointer to data buffer
    \param bytes  Number of bytes to read
    \param index Position of the incoming byte in hte receive buffer 
    \return 1 if successful, otherwise 0
 */
char i2c_read_byte(unsigned char* rcvdata, unsigned char bytes, unsigned char index)
{
        unsigned char byte = 0;
	unsigned char bit = 0;
	//release SDA
	SET_SDA_IN();
	for (bit = 0; bit < 8; bit++) 
	{
              toggle_scl();//goes high
              if(READ_SDA())
                      byte|= (1 << (7- bit));
              __delay_cycles(DELAY);
              toggle_scl();//goes low
              __delay_cycles(DELAY);
        }
	rcvdata[index] = byte;
	//take SDA
	SET_SDA_OUT();
	if(index < (bytes-1))
	{
		write_sda(0);
		toggle_scl(); //goes high for the 9th clock
		__delay_cycles(DELAY);
		//Pull SCL low
		toggle_scl(); //end of byte with acknowledgment. 
		//release SDA
		write_sda(1);
		__delay_cycles(DELAY);
	}
	else //send NACK on the last byte
	{
		write_sda(1);
		toggle_scl(); //goes high for the 9th clock
		__delay_cycles(DELAY);
		//Pull SCL low
		toggle_scl(); //end of byte with acknowledgment. 
		//release SDA
		__delay_cycles(DELAY);
	}		
	return 1;
		
}	
/*! \brief Writes SCL.
    \param x tristates SCL when x = 1, other wise 0
 */
void write_scl (char x)
{
      if(x)
      {
            DDR_SCL &= ~(1 << SCL); //tristate it
	    //check clock stretching
	    while(!READ_SCL());
      }
      else
      {
            DDR_SCL |= (1 << SCL); //output 
            PORT_SCL &= ~(1 << SCL); //set it low
            
      }
}

/*! \brief Writes SDA.
    \param x tristates SDA when x = 1, other wise 0
 */
void write_sda (char x)
{
      if(x)
      {
            DDR_SDA &= ~(1 << SDA); //tristate it
      }
      else
      {
            DDR_SDA |= (1 << SDA); //output 
            PORT_SDA &= ~(1 << SDA); //set it low
            
      }
}
/*! \brief Toggles SCL.
 */
void toggle_scl() 
{
	if(PIN_SCL & (1<<SCL))
	{
		DDR_SCL |= (1 << SCL); //output
		PORT_SCL &= ~(1 << SCL); //set it low
	}
	else
	{
		DDR_SCL &= ~(1 << SCL); //tristate it
		while(!READ_SCL());
	}		
}
