#include <xc.h>
#include <stdint.h>
//uint8_t eepromRefresh();
uint8_t eepromRead(uint16_t addr)
{
    EEADR = addr;
    EEPGD = 0;
    CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}
void eepromWrite(uint16_t addr, uint8_t data)
{
    uint8_t GIE_temp;
    
    EEADR = addr;
    EEDATA = data;
    EEPGD = 0;
    CFGS = 0;
    WREN = 1;
    //Required sequence
    GIE_temp = GIE;
    //
    GIE=0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    //
    GIE = GIE_temp;
    
    WREN = 0;
}

double f = 15.6;
uint8_t *pf;
double e = 0;
double *pe;
uint8_t rambuf[5];

void test01(void)
{  
    pf = (uint8_t*)&f;
    
    eepromWrite(0x00 , *pf);
    eepromWrite(0x01, *(pf+1));
    eepromWrite(0x02, *(pf+2));
    
    rambuf[0]= eepromRead(0);
    rambuf[1]= eepromRead(1);
    rambuf[2]= eepromRead(2);
    
    pe = (double *) rambuf;
    e = *pe;
    PORTA = PORTB * e;
    
}



