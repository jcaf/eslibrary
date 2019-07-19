/* 
 * File:   eeprom.h
 * Author: jcaf
 *
 * Created on July 18, 2019, 7:26 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

void eepromWrite(uint16_t addr, uint8_t data);
uint8_t eepromRead(uint16_t addr);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

