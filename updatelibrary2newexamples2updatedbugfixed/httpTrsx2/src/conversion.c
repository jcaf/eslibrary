/*
 * conversion.c
 *
 *  Created on: Sep 16, 2019
 *      Author: jcaf
 */

#include <stdint.h>
#include <stdlib.h>
#include "conversion.h"

void cUint32ToStr(uint32_t ui32, char *buff, size_t size)
{
	snprintf(buff, size, "%lu", ui32);
}

void cIntToStr(int i, char *outbuff, size_t size)
{
	//#if defined(__AVR__) && defined(__GNUC__)
	//	  itoa( i, outbuff, 10 );//it's no safe to unsigned longs
    //#else
		snprintf(outbuff, size, "%i", i);
    //#endif
}
void cDoubleToStr(double f, uint8_t numDec, char *outbuff, size_t size)//, size_t outbuffSize)
{
	#if defined(__AVR__) && defined(__GNUC__)
    	dtostrf(f, 0, numDec, outbuff);
	#else
        //sprintf(outbuff, "%.2f", f);
        snprintf(outbuff, size, "%.2f", f);
	#endif
}
double cStrToDouble(char *str)
{
	return atof(str);
	//return strtod(str, NULL);
}
long cStrToLong(char *str)
{
    //strtol()
    return atol(str);
}
int cStrToInt(char *str)
{
    //strtol()
    return atoi(str);
}

/*
uint16_t json_cFloatStrArr_To_DoubleArr(char *c, double *d, uint16_t n)
{
    uint16_t count = 0x0;
    uint8_t i = 0x0;
    char buff[20];

    uint16_t length = strlen(c);
    if ((length < 3) || (n==0))// ([] = 2chars) <3 caracteres-> vacio->return
    {
        *d = 0;
    }
    else//[x] al menos hay un caracter
    {
        c[length-1] = '\0'; //chunck ']' by '\0'
        c++;                //beging gnoring '['
        do
        {
            if ((*c == ',') || (*c == '\0'))
            {
                buff[i] = '\0';
                d[count++] = cStrToDouble(buff);
                //
                i = 0x00;
                if ( (*c == '\0') || (count >= n))
                    break;
            }
            else
            {
                buff[i++] = *c;
            }
            c++;
        }while (1);
    }
    return count;
}
*/

