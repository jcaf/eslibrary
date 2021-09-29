/*
 * conversion.h
 *
 *  Created on: Sep 16, 2019
 *      Author: jcaf
 */

#ifndef CONVERSION_H_
#define CONVERSION_H_
#include <stdint.h>

void cUint32ToStr(uint32_t ui32, char *buff, size_t size);
void cIntToStr(int i, char *outbuff, size_t size);
void cDoubleToStr(double f, uint8_t numDec, char *outbuff, size_t size);
//
double cStrToDouble(char *str);
long cStrToLong(char *str);
int cStrToInt(char *str);

//uint16_t json_cFloatStrArr_To_DoubleArr(char *c, double *d, uint16_t n);



#endif /* CONVERSION_H_ */
