/*
 * json.h
 *
 *  Created on: Aug 29, 2019
 *      Author: jcaf
 */

#ifndef JSON_H_
#define JSON_H_

typedef struct _JSON
{
	char *name;
	char *strval;
} JSON;

void 	json_cFloatArr(float *v, int size, char *outbuff);
int8_t 	json_decode(char *stream, size_t streamSize, JSON *json);
enum
{
	CNUM_FLOAT,
	CNUM_DOUBLE,
	CNUM_INT,
	CNUM_UINT8,
	CNUM_LONG
};
void 	json_cNumericArrToStr(void *v, int8_t typeData, int size, char *outbuff);
void 	json_cNumericBiArrToStr(void *v, int8_t typeData, int sizeX, int sizeY, char *buff);
uint16_t json_getContentLength(JSON *json, uint16_t npairs);

uint16_t json_cNumericStrArr_To_NumericArr(char *vchar, int8_t typeData, void *vnum, uint16_t numElem);

//Use defines:
#define json_cDecStrArr_To_DoubleArr(vchar,vdouble,numElem) json_cNumericStrArr_To_NumericArr(vchar, CNUM_DOUBLE, (void*)vdouble,numElem)
#define json_cLongStrArr_To_LongArr(vchar,vlong,numElem) json_cNumericStrArr_To_NumericArr(vchar, CNUM_LONG, (void*)vlong,numElem)
#define json_cIntStrArr_To_IntArr(vchar,vint,numElem) json_cNumericStrArr_To_NumericArr(vchar, CNUM_INT, (void*)vint,numElem)
//
uint16_t json_cCstrToStrMatrix(char *c, char *outbuff, uint8_t numStr, size_t sizeStr);


#endif /* JSON_H_ */
