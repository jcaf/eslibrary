/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: jcaf
 *
 * Created on August 17, 2019, 4:59 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _JSON
{
	char *name;
	char *txtval;
}JSON;

//void json_cIntegerArr
void json_cFloatArr(float *v, int size, char *outbuff)
{
    char buff[10];
    
    int i=0;
    strcpy(outbuff,"[");
    
    for (i=0; i< size; i++)
    {
        //sprintf(buff, "%.2f", v[i]);
        snprintf(buff,10, "%.2f", v[i]);
        
        strcat(outbuff, buff);//alternatevly use strlcat
        
        if (i != size-1)
            strcat(outbuff, ",");
    }
    strcat(outbuff,"]");
}
//en otro punto voy armando el json        
void setup(void)
{	
    int i;
    //
    #define JSON_SIZEMAX 4
    JSON json[JSON_SIZEMAX];
    //
    json[0].name = "hello";
    char analog0[10];
    sprintf(analog0, "%.2f", 12.3 );//sprintf(analog0, "\"%.2f\"", 12.3 );
    json[0].txtval = analog0;
    
    //
    json[1].name = "ch";
    float volt[8] = {1,2,3,4,5,6,7,8};//COMO LO DESEA CONVERTIR el usuario, el es libre de hacer lo que quiera...
    char volt_str[100];
    json_cFloatArr(volt, 8 , volt_str);
    json[1].txtval = volt_str;
    //a las finales yo tengo que tener el json en un texto final
    
    //debug
    //for (i=0; i<JSON_SIZEMAX; i++)
    for (i=0; i<2; i++)
    {
        printf("\"%s\":%s\n", json[i].name, json[i].txtval);
    }
}
void loop(void)
{

}
int main(int argc, char** argv) 
{
    setup();
    

    return (EXIT_SUCCESS);
}

