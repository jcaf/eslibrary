/*
 * json.c
 *
 *  Created on: Aug 29, 2019
 *      Author: jcaf
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "json.h"
#include "mystring.h"
#include "conversion.h"


uint16_t json_cNumericStrArr_To_NumericArr(char *vchar, int8_t typeData, void *vnum, uint16_t numElem)
{
    uint16_t count = 0x0;
    uint8_t i = 0x0;
    char buff[20];

    uint16_t length = strlen(vchar);
    if ((length >= 3) && (numElem>0)) // ([] = 2chars) <3 caracteres-> vacio->return
    {                           //[x] al menos hay un caracter
        vchar[length-1] = '\0';     //chunck ']' by '\0'
        vchar++;                    //beging gnoring '['
        do
        {
            if ((*vchar == ',') || (*vchar == '\0'))
            {
                buff[i] = '\0';

                if (typeData == CNUM_DOUBLE)
                    ((double*)vnum)[count++] = cStrToDouble(buff);
                else if (typeData == CNUM_INT)
                {
                    ((int*)vnum)[count++] = (int)cStrToInt(buff);
                }
                else if (typeData == CNUM_LONG)
                {
                    ((long*)vnum)[count++] = cStrToLong(buff);
                }

                //
                i = 0x00;
                if ( (*vchar == '\0') || (count >= numElem))
                    break;
            }
            else
            {
                buff[i++] = *vchar;
            }
            vchar++;
        }while (1);
    }
    return count;
}

/*
numStr : Num. of C-strings to place in outbuff
sizeStr : Size of placeholder by each C-string
*/
uint16_t json_cCstrToStrMatrix(char *c, char *outbuff, uint8_t numStr, size_t sizeStr)
{
    uint16_t count = 0x0;
    char *pi;
    uint16_t length = strlen(c);
    if ((length >= 3) || (numStr>0) || (sizeStr>0) )// ([] = 2chars) <3 caracteres-> vacio->return
    {
        //c[length-1] = '\0'; //chunck ']' by '\0'
        c++;                //beging gnoring '['
        pi = c;
        while (*c != '\0')
        {
            if ((*c == ',') || (*c == ']'))
            {
                *c = '\0';
                strncpy(outbuff, pi, sizeStr);
                outbuff+= sizeStr;
                pi = c+1;//next
                //
                if (++count >= numStr)
                    break;
            }
            c++;
        }
    }
    return count;
}



void json_cNumericArrToStr(void *v, int8_t typeData, int size, char *outbuff)
{
	char buff[20];
	int i;
	strcpy(outbuff, "[");

	if (typeData == CNUM_FLOAT)
	{
		for (i = 0; i < size; i++)
		{
			cDoubleToStr(((float*) v)[i], 2, buff, sizeof(buff)); //2 decimals
			//
			strcat(outbuff, buff);            //alternatevly use strlcat
			if (i < size - 1)
				strcat(outbuff, ",");
		}
	}
	else if (typeData == CNUM_INT)
	{
		for (i = 0; i < size; i++)
		{
			cIntToStr(((int*) v)[i], buff, sizeof(buff));
			//
			strcat(outbuff, buff);
			if (i < size - 1)
				strcat(outbuff, ",");
		}
	}
	else if (typeData == CNUM_UINT8)
	{
		for (i = 0; i < size; i++)
		{
			cIntToStr(((uint8_t*) v)[i], buff, sizeof(buff));
			//
			strcat(outbuff, buff);
			if (i < size - 1)
				strcat(outbuff, ",");
		}
	}
	strcat(outbuff, "]");
}

void json_cNumericBiArrToStr(void *v, int8_t typeData, int sizeX, int sizeY, char *buff)
{
	void *pv;
	int i;
	//
	strcpy(buff, "[");
	pv = v;
	for (i = 0; i < sizeX; i++)
	{
		json_cNumericArrToStr(pv, typeData, sizeY, (buff + strlen(buff)));

		if (typeData == CNUM_FLOAT)
			pv = (float*) pv + sizeY;
		else if (typeData == CNUM_INT)
			pv = (int*) pv + sizeY;
		else if (typeData == CNUM_UINT8)
			pv = (uint8_t*) pv + sizeY;

		//
		if (i < sizeX - 1)
			strcat(buff, ",");
	}
	strcat(buff, "]");
}

uint16_t json_getContentLength(JSON *json, uint16_t npairs)
{
	uint16_t acc = 0;
	uint16_t i;
	for (i = 0; i < npairs; i++)
	{
		acc += strlen(json[i].name) + strlen(json[i].strval) + 3;    //+2"" + 1:
		if (i < (npairs - 1))
		{
			acc++;
		}  //comma
	}
	return acc + 2;  //+2 curl-braces
}

int8_t json_decode(char *stream, size_t streamSize, JSON *json)
{
	static size_t i = 0;
	//int16_t
	size_t iname = 0;
	//int16_t
	size_t ivalue = 0;
	int8_t cod_ret = 0;

	int8_t sm0 = 0;
	int8_t sm1 = 0;

	while (1)
	{
		if (sm0 == 0)
		{
			if (stream[i] == '\"')
			{
				iname = i + 1;  //save initial position
				sm0++;
			}
		}
		else if (sm0 == 1)  //cannot be " or any diferent to 'A-Z''a'b or"0.9"
		{
			if (stream[i] == '\"')  //= 'A-Z') ) //letras y numeros
				sm0 = 0x00;
			else
				sm0++;
		}
		else if (sm0 == 2)
		{
			if (stream[i] == '\"')
			{
				sm0++;
			}
			else
			{
				if (!charIsLetter(stream[i]) && !charIsNumber(stream[i]))
				{
					sm0 = 0x00;
				}
			}
		}
		else if (sm0 == 3)
		{
			if (stream[i] == ':')
			{
				stream[i - 1] = '\0';
				sm0++;
			}
			else
			{
				sm0 = 0;
			}
		}
		else if (sm0 == 4)
		{
			if (stream[i] == '\"')
			{
				ivalue = i;  //+1;//suprim "
				sm0 = 5;  //string
			}
			else if (charIsNumber(stream[i]))
			{
				ivalue = i;
				sm0 = 6; //number
			}
			else if (stream[i] == '[')
			{
				ivalue = i;
				sm0 = 7;
			}
			else
			{
				sm0 = 0x00;
			}
		}
		//string----------------------------
		else if (sm0 == 5)
		{
			if (sm1 == 0)
			{
				if (stream[i] == '\"')
				{
					sm1++;
				}
			}
			else if (sm1 == 1)
			{
				if ((stream[i] == ',') || (stream[i] == '}'))
				{
					stream[i] = '\0';
					sm0 = 8;
				}
			}
		}
		//number----------------------------
		else if (sm0 == 6)
		{
			if ((stream[i] == ',') || (stream[i] == '}'))
			{
				stream[i] = '\0';
				sm0 = 8;
			}
			//Este else podria salir y dejar a responsabilidad del Sender
			else
			{
				if ((stream[i] != '.') && (!charIsNumber(stream[i])))
				{
					sm0 = 0;
					sm1 = 0;
				}
			}
		}
		//vector----------------------------
		else if (sm0 == 7)
		{
			if (sm1 == 0)
			{
				if (stream[i] == ']')
				{
					sm1++;
				}
			}
			else if (sm1 == 1)
			{
				if ((stream[i] == ',') || (stream[i] == '}'))
				{
					stream[i] = '\0';
					sm0 = 8;
				}
			}
		}
		//exit
		if (sm0 == 8)
		{
			json->name = &stream[iname];
			json->strval = &stream[ivalue];
			sm1 = 0;
			sm0 = 0;
			break;
		}
		//while end
		if (++i >= streamSize)
		{
			i = 0x00;
			sm0 = 0x0;
			sm1 = 0x0;
			cod_ret = 1;
			break;
		}
	}
	return cod_ret;
}
