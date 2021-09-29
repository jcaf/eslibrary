/*
 * mystring.c
 *
 *  Created on: Sep 16, 2019
 *      Author: jcaf
 */
#include <stdlib.h>
#include <stdint.h>
#include "mystring.h"

int8_t charIsNumber(char c)
{
	if ((c>='0') && (c<='9'))
		return 1;
	else
		return 0;
}

int8_t charIsLetter(char c)
{
	if ( ( (c>='A') && (c<='Z')) || ( (c>='a') && (c<='z') ) )
		return 1;
	else
		return 0;
}
