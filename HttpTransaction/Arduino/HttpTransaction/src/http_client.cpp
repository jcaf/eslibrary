#include "http_client.h"
#include "ethernet.h"           //C++ header
#include "tcp_client_connect.h" //C++ header
#include <string.h>
#include <stdint.h>

#if defined(__AVR__) && defined(__GNUC__)
#include <Arduino.h>
#include <Ethernet.h>

#define __millis() millis()
#define tcp_client_available()  client.available()
#define tcp_client_connected()  client.connected()
#define tcp_client_stop()       client.stop()
#define http_client_read_char() client.read()

//ram
#define http_send_data(d) do{client.print(d);\
        nt_debug_print(d);\
    }while(0)

//prgmem = flash
//#define http_send_data_prgmem(d) do{client.print( reinterpret_cast<const __FlashStringHelper *> (d) );\
//                                    nt_debug_print_prgmem( reinterpret_cast<const __FlashStringHelper *> (d) );\
//                                }while(0)


#ifdef NT_DEBUG_PRINT
    #define http_send_data_prgmem(d) do{client.print( reinterpret_cast<const __FlashStringHelper *> (d) );\
                                        nt_debug_print_prgmem( reinterpret_cast<const __FlashStringHelper *> (d) );\
                                    }while(0)

#else
    #define http_send_data_prgmem(d) do{client.print( reinterpret_cast<const __FlashStringHelper *> (d) );}while(0)

#endif



#define http_send_data_f(d) do{client.print(d, FLOAT_N_DECIMALS);\
        nt_debug_print_float(d);\
    }while(0)


#define http_send_data_char(d) http_send_data(d)
#define http_send_data_int(d) http_send_data(d)
#define http_send_data_short(d) http_send_data(d)
#define http_send_data_long(d) http_send_data(d)
#define http_send_data_float(d) http_send_data_f(d)
#define http_send_data_double(d) http_send_data_f(d)

#define http_send_data_cstring(d) http_send_data(d)//ram
#define http_send_data_cstring_prgmem(d) http_send_data_prgmem(d)//prgmem

//
#define http_send_data_uint(d) http_send_data(d)
#define http_send_data_ushort(d) http_send_data(d)
#define http_send_data_ulong(d) http_send_data(d)
#else //__GNUC__
#include <stdio.h>

#define http_send_data_char(d) printf(PSTR("%c", d)
#define http_send_data_int(d) printf(PSTR("%i", d)
#define http_send_data_short(d) printf(PSTR("%hi", d)
#define http_send_data_long(d) printf(PSTR("%li", d)
#define http_send_data_float(d) printf(FLOAT_FORMAT_SPEC, d)
#define http_send_data_double(d) printf(FLOAT_FORMAT_SPEC, d)
#define http_send_data_cstring(d) printf(PSTR("%s", d)
//
#define http_send_data_uint(d) printf(PSTR("%u", d)
#define http_send_data_ushort(d) printf(PSTR("%lu", d)
#define http_send_data_ulong(d) printf(PSTR("%lu", d)

int dummy_ret_true(void)
{
    return 1;
}
#define __millis()              dummy_ret_true()
#define tcp_client_available()  dummy_ret_true()
#define tcp_client_connected()  dummy_ret_true()
#define tcp_client_stop()       dummy_ret_true()
#define http_client_read_char() dummy_ret_true()

#endif

void http_send_rqstline(void);
void http_send_hdrlines(void);
void http_send_msgbody(void);

#include "iot/http_msg_body.h"
extern const struct http_msgbody_register_t msgbody[] PROGMEM;

void nn(void)
{
char buff[20];

	strcpy_P(buff, (const char *) &msgbody[1].name);
    int *pint= (int *) pgm_read_word(&(msgbody[1].addr));

    Serial.println(buff);
    //Serial.println(*pint);
}
//Constant strings in Program Space
const PROGMEM char kstr_NL[]="\r\n";

int8_t http_trx_request_msg(void)//send the request message to HTTP server
{
    int8_t cod_ret = 0;
    //int8_t
    HTTP_RQSTLINE_METHOD_E rqst_method = http_rqstline_get_method();

    http_send_rqstline();
    http_send_hdrlines();
    http_send_data_cstring_prgmem(kstr_NL);//"\r\n"
    if (rqst_method == POST_E)
    {
        http_send_msgbody();
        http_send_data_cstring_prgmem(kstr_NL);//"\r\n"
    }
    cod_ret = 1;
    return cod_ret;
}
const PROGMEM char kstr_GET[]="GET ";
const PROGMEM char kstr_POST[]="POST ";
const PROGMEM char kstr_HTTP_1_0[]=" HTTP/1.0";
const PROGMEM char kstr_HTTP_1_1[]=" HTTP/1.1";
const PROGMEM char kstr_QUESTIONMARK[]="?";

void http_send_rqstline(void)
{
    //int8_t
    HTTP_RQSTLINE_METHOD_E rqst_method;
    const char *URIpath;
    int8_t http_version;

    rqst_method = http_rqstline_get_method();
    if (rqst_method == GET_E)
    {
        http_send_data_cstring_prgmem(kstr_GET);//"GET "
    }
    else if (rqst_method == POST_E)
    {
        http_send_data_cstring_prgmem(kstr_POST);//"POST "
    }
    URIpath = http_rqstline_get_URIpath();
    http_send_data_cstring(URIpath);
    //http_send_data_cstring_prgmem(URIpath);

    if (rqst_method == GET_E)
    {
        if (body_register_get_num_regs()>0)//envia en x-form-urlencoded
        {
            http_send_data_cstring_prgmem(kstr_QUESTIONMARK);//there is a query string to send
        }
    }

    http_version = http_rqstline_get_http_version();
    if (http_version == HTTP_1_0_E)
        http_send_data_cstring_prgmem(kstr_HTTP_1_0);//" HTTP/1.0"
    else if (http_version == HTTP_1_1_E)
        http_send_data_cstring_prgmem(kstr_HTTP_1_1);//" HTTP/1.1"

    http_send_data_cstring_prgmem(kstr_NL);//"\r\n"
}

void http_hdrlines_send_ContentType(void);
void http_hdrlines_send_ContentLength(void);
void http_hdrlines_send_Host(void);
void http_hdrlines_send_UserAgent(void);
void http_hdrlines_send_Connect(void);

void http_send_hdrlines(void)
{
    //int8_t
    HTTP_RQSTLINE_METHOD_E rqst_method = http_rqstline_get_method();

    http_hdrlines_send_Host();
    http_hdrlines_send_Connect();

    if ((rqst_method == POST_E)||(rqst_method == PUT_E))
    {
        http_hdrlines_send_ContentType();
    }


    if (http_hdrlines.lines != NULL)//free header is set via char string
    {
        http_send_data_cstring(http_hdrlines.lines);
    }

    http_hdrlines_send_UserAgent();

    if ((rqst_method == POST_E)||(rqst_method == PUT_E))
    {
        http_hdrlines_send_ContentLength();
    }
}

const PROGMEM char kstr_CONTENT_TYPE[]="Content-Type: ";
const PROGMEM char kstr_APPLICATION_JSON[]="application/json";
const PROGMEM char kstr_APPLICATION_XML[]="application/xml";

void http_hdrlines_send_ContentType(void)
{
    HTTP_HDR_CONTENT_TYPE_E ContentType;
    ContentType = http_hdrlines_get_ContentType();

    if (ContentType != 0)
    {
        http_send_data_cstring_prgmem(kstr_CONTENT_TYPE);//"Content-Type: "
        if (ContentType == APPLICATION_JSON_E)
        {
            http_send_data_cstring_prgmem(kstr_APPLICATION_JSON);//"application/json"
        }
        else
        {
            http_send_data_cstring_prgmem(kstr_APPLICATION_XML);//"application/xml"
        }

        http_send_data_cstring_prgmem(kstr_NL);//"\r\n"
    }
}
const PROGMEM char kstr_CONTENT_LENGTH[]="Content-Length: ";
const PROGMEM char kstr_ZERO[]="0";

int32_t get_body_length(void);

void http_hdrlines_send_ContentLength(void)
{
    char str[20];
    int32_t ContentLength;

    ContentLength = http_hdrlines_get_ContentLength();
    int8_t ok;
    if (1)//ContentLength != 0)
    {
        http_send_data_cstring_prgmem(kstr_CONTENT_LENGTH);//"Content-Length: "
        if (ContentLength < 0)//== CONTENT_LENGTH_DYNAMIC)
        {
            ContentLength = get_body_length();
            ok = snprintf(str, 20, "%u", ContentLength);//_itoa(ContentLength, str, 10);
            if (ok>=0 && ok <20)
                http_send_data_cstring(str);
            else
                http_send_data_cstring_prgmem(kstr_ZERO);//"0"

        }
        else //set from 0 to int16_t_max
        {
            ok = snprintf(str, 20, "%u", ContentLength);//_itoa(ContentLength, str, 10);
            if (ok>=0 && ok <20)
                http_send_data_cstring(str);
            else
                http_send_data_cstring_prgmem(kstr_ZERO);
        }
        http_send_data_cstring_prgmem(kstr_NL);//"\r\n"
    }
}

const PROGMEM char kstr_HOST[]="Host: ";

void http_hdrlines_send_Host(void)
{
    const char *Host = http_hdrlines_get_Host();
    if (Host != NULL)
    {
        http_send_data_cstring_prgmem(kstr_HOST);//"Host: "
        http_send_data_cstring(Host);
        http_send_data_cstring_prgmem(kstr_NL);
    }
}

const PROGMEM char kstr_USER_AGENT[]="User-Agent: ";

void http_hdrlines_send_UserAgent(void)
{
    const char *UserAgent = http_hdrlines_get_UserAgent();
    if (UserAgent != NULL)
    {
        http_send_data_cstring_prgmem(kstr_USER_AGENT);//"User-Agent: "
        http_send_data_cstring(UserAgent);
        http_send_data_cstring_prgmem(kstr_NL);//"\r\n"
    }
}

const PROGMEM char kstr_CONNECTION[]="Connection: ";
const PROGMEM char kstr_CLOSE_NL[]="close\r\n";
const PROGMEM char kstr_KEEP_ALIVE_NL[]="keep-alive\r\n";

void http_hdrlines_send_Connect(void)
{
    HTTP_HDR_CONNECTION_E connect = http_hdrlines_get_Connection();

    http_send_data_cstring_prgmem(kstr_CONNECTION);//"Connection: "
    if (connect == CONNECTION_CLOSE_E)
    {
        http_send_data_cstring_prgmem(kstr_CLOSE_NL);//"close\r\n"
    }
    else// KEEP_ALIVE_E
    {
        http_send_data_cstring_prgmem(kstr_KEEP_ALIVE_NL);//"keep-alive\r\n" //HTTP persistent connection
    }
}
/******************************************************************************************
MESSAGE BODY
******************************************************************************************/
void rsqt_method_POST(void);

void http_send_msgbody(void)
{
    rsqt_method_POST();
}

//POST method
void http_send_typedata(void * addr, int8_t type);
void http_send_array_typedata(void * addr, int8_t type, uint16_t idx);
void rsqt_method_POST(void)
{
    char *name;
    int8_t type;
    void * addr;
    sizedata_t size;
    //
    int8_t num_regs = body_register_get_num_regs();
    int8_t reg;
    uint16_t idx;
    //
    http_send_data_char('{');
    for ( reg=0; reg< num_regs; reg++)
    {
        name = body_register_get_name(reg);
        type = body_register_get_type(reg);
        addr = body_register_get_addr(reg);
        size = body_register_get_size(reg);

        //print name
        http_send_data_char('\"');
        http_send_data_cstring(name);
        http_send_data_char('\"');
        //
        http_send_data_char(':');
        //
        //print value
        if ( (type > TD_BEGIN) && (type < TD_END))
        {
            http_send_typedata(addr, type);//by_type
        }
        else if (type == CSTRING_TD)
        {
            http_send_data_char('\"');
            http_send_data_cstring((char *) addr);
            http_send_data_char('\"');

        }
        else if ( (type > ARRAY_TD_BEGIN) && (type < ARRAY_TD_END))
        {
            http_send_data_char('[');
            for (idx=0; idx<size; idx++)
            {
                http_send_array_typedata(addr, type, idx);

                if (idx< (size-1))
                {
                    http_send_data_char(',');//separador [,,,]
                }
            }
            http_send_data_char(']');
        }
        //
        if (reg< (num_regs-1))
        {
            http_send_data_char(',');//existe siguiente dato x enviar
            //http_send_data_char('\n');//only for confortable view in PC
        }
    }
    http_send_data_char('}');//end without \r\n
}


void http_send_typedata(void * addr, int8_t type)
{
    char char_temp;

    switch (type)
    {
        //signeds:
        case CHAR_TD:
            http_send_data_char('\"');
            char_temp = * (char *)addr;
            if (
                ((char_temp>= ' ') && (char_temp <= '~'))//printables chars from 32 to 126
                ||(char_temp=='\"') // \": double quote (U+0022 QUOTATION MARK)
                ||(char_temp=='\\') // \\: backslash (U+005C REVERSE SOLIDUS)
                //||(char_temp=='/') // \/: slash (U+ ? SOLIDUS)
                ||(char_temp=='\b') // \b: backspace (U+0008 BACKSPACE)
                ||(char_temp=='\f') // \f: form feed (U+000C FORM FEED)
                ||(char_temp=='\n') // \n: line feed (U+000A LINE FEED)
                ||(char_temp=='\r') // \r: carriage return (U+000D CARRIAGE RETURN)
                ||(char_temp=='\t') // \t: horizontal tab (U+0009 CHARACTER TABULATION)
            )
            {

                http_send_data_char(char_temp);

            }
            else
            {
                //or send \u 0000 hex code
                http_send_data_char(' ');//blank space
            }
            http_send_data_char('\"');
            break;

        case INT_TD:
            http_send_data_int(* (int *)addr);
            break;

        case SHORT_TD:
            http_send_data_short(* (short *)addr);
            break;

        case LONG_TD:
            http_send_data_long(* (long *)addr);
            break;

            #ifdef LONLONG_TD_AVAILABLE
        case LONGLONG_TD:
            http_send_data_long(* (long long *)addr);
            break;
            #endif
        //unsigneds:
        case UCHAR_TD://send as integer
            http_send_data_uint(* (unsigned char *)addr);
            break;

        case UINT_TD:
            http_send_data_uint(* (unsigned int *)addr);
            break;
        case USHORT_TD:
            http_send_data_uint(* (unsigned short *)addr);
            break;

        case ULONG_TD:
            http_send_data_uint(* (unsigned long *)addr);
            break;

            #ifdef LONLONG_TD_AVAILABLE
        case ULONGLONG_TD:
            http_send_data_uint(* (unsigned long long *)addr);
            break;
            //
            #endif // LONLONG_TD_AVAILABLE
        case FLOAT_TD:
            http_send_data_float(* (float*)addr);
            break;

        case DOUBLE_TD:
            http_send_data_float(* (double*)addr);
            break;
        //
        case INT8_T_TD:
            http_send_data_int(* (int8_t *)addr);
            break;

        case INT16_T_TD:
            http_send_data_int(* (int16_t *)addr);
            break;

        case INT32_T_TD:
            http_send_data_int(* (int32_t *)addr);
            break;
            #ifdef INT64T_TD_AVAILABLE
        case INT64_T_TD:
            http_send_data_int(* (int64_t *)addr);
            break;
            #endif // INT64T_TD_AVAILABLE
        //
        case UINT8_T_TD:
            http_send_data_uint(* (uint8_t *)addr);
            break;

        case UINT16_T_TD:
            http_send_data_uint(* (uint16_t *)addr);
            break;

        case UINT32_T_TD:
            http_send_data_uint(* (uint32_t *)addr);
            break;
            #ifdef INT64T_TD_AVAILABLE
        case UINT64_T_TD:
            http_send_data_uint(* (uint64_t *)addr);
            break;
            #endif // INT64T_TD_AVAILABLE
        default:
            break;
    }
}


void http_send_array_typedata(void * addr, int8_t type, uint16_t idx)
{
    char char_temp;

    switch (type)
    {
        case CHAR_ARRAY_TD://solo los caracteres printables+

            http_send_data_char('\"');

            char_temp = ((char *)addr)[idx];
            if (
                ((char_temp>= ' ') && (char_temp <= '~'))//32 al 126
                ||(char_temp=='\"')
                ||(char_temp=='\\')
                //||(char_temp=='/')
                ||(char_temp=='\b')
                ||(char_temp=='\f')
                ||(char_temp=='\n')
                ||(char_temp=='\r')
                ||(char_temp=='\t')
            )
            {
                http_send_data_char(char_temp);
            }
            else
            {
                http_send_data_char(' ');//blank space
            }
            http_send_data_char('\"');
            break;

        case INT_ARRAY_TD:

            http_send_data_int( ((int *)addr)[idx]);
            break;

        case SHORT_ARRAY_TD:

            http_send_data_short( ((short *)addr)[idx]);
            break;

        case LONG_ARRAY_TD:

            http_send_data_long( ((long *)addr)[idx]);
            break;

            #ifdef LONLONG_TD_AVAILABLE
        case LONGLONG_ARRAY_TD:

            http_send_data_long( ((long long *)addr)[idx]);
            break;
            #endif // LONLONG_TD_AVAILABLE

        //unsigneds:
        case UCHAR_ARRAY_TD://como un entero

            http_send_data_uint( ((unsigned char *)addr)[idx]);
            break;

        case UINT_ARRAY_TD:

            http_send_data_uint( ((unsigned int *)addr)[idx]);
            break;

        case USHORT_ARRAY_TD:

            http_send_data_uint( ((unsigned short *)addr)[idx]);
            break;

        case ULONG_ARRAY_TD:

            http_send_data_uint( ((unsigned long *)addr)[idx]);
            break;

            #ifdef LONLONG_TD_AVAILABLE
        case ULONGLONG_ARRAY_TD:

            http_send_data_uint( ((unsigned long long *)addr)[idx]);
            break;
            #endif // LONLONG_TD_AVAILABLE
        //
        case FLOAT_ARRAY_TD:

            http_send_data_float( ((float*)addr)[idx]);
            break;

        case DOUBLE_ARRAY_TD:

            http_send_data_float( ((double*)addr)[idx]);
            break;
        //
        case INT8_T_ARRAY_TD:

            http_send_data_int( ((int8_t *)addr)[idx]);
            break;

        case INT16_T_ARRAY_TD:

            http_send_data_int( ((int16_t *)addr)[idx]);
            break;

        case INT32_T_ARRAY_TD:

            http_send_data_int( ((int32_t *)addr)[idx]);
            break;
            #ifdef INT64T_TD_AVAILABLE
        case INT64_T_ARRAY_TD:

            http_send_data_int( ((int64_t *)addr)[idx]);
            break;
            #endif // INT64T_TD_AVAILABLE
        //
        case UINT8_T_ARRAY_TD:

            http_send_data_uint( ((uint8_t *)addr)[idx]);
            break;

        case UINT16_T_ARRAY_TD:

            http_send_data_uint( ((uint16_t *)addr)[idx]);
            break;

        case UINT32_T_ARRAY_TD:

            http_send_data_uint( ((uint32_t *)addr)[idx]);
            break;
            #ifdef INT64T_TD_AVAILABLE
        case UINT64_T_ARRAY_TD:

            http_send_data_uint( ((uint64_t *)addr)[idx]);
            break;
            #endif // INT64T_TD_AVAILABLE
        default:
            break;
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#include <limits.h>
/*
int numPlaces (int n)
{
    //    if (n < 0)
    //        n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 10)
        return 1;
    if (n < 100)
        return 2;
    if (n < 1000)
        return 3;
    if (n < 10000)
        return 4;
    if (n < 100000)
        return 5;
    if (n < 1000000)
        return 6;
    if (n < 10000000)
        return 7;
    if (n < 100000000)
        return 8;
    if (n < 1000000000)
        return 9;

    //2147483647 is 2^31-1 - add more ifs as neededand adjust this final return as well.
    return 10;
}

int uints_get_numb_digits(uint32_t n)
{
    if (n < 10)
        return 1;
    if (n < 100)
        return 2;
    if (n < 1000)
        return 3;
    if (n < 10000)
        return 4;
    if (n < 100000)
        return 5;
    if (n < 1000000)
        return 6;
    if (n < 10000000)
        return 7;
    if (n < 100000000)
        return 8;
    if (n < 1000000000)
        return 9;


    return 10;
}

int int_n_digits_inc_sign(int n)//numero de digitos & signo _inc_sign
{
    uint8_t s=0;//sign
    if (n < 0)
    {
        n = (n == INT_MIN) ? INT_MAX : -n;
        s =1;
    }
    return numPlaces(n) + s;
}

int float_find_number_of_digits(float f, uint8_t d)//float & double
{
    //sd=sign+digits
    int sd = int_n_digits_inc_sign( (int) f);
    return sd + 1 + d;	//iii.dd
}
*/
uint8_t uintmax_t_find_number_of_digits(uintmax_t n)
{
	//    if (n < 0)
    //        n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 1E1)
        return 1;
    if (n < 1E2)
        return 2;
    if (n < 1E3)
        return 3;
    if (n < 1E4)
        return 4;
    if (n < 1E5)
        return 5;
    if (n < 1E6)
        return 6;
    if (n < 1E7)
        return 7;
    if (n < 1E8)
        return 8;
    if (n < 1E9)
        return 9;
    if (n < 1E10)
        return 10;
    if (n < 1E11)
        return 11;
    if (n < 1E12)
        return 12;
    if (n < 1E13)
        return 13;
    if (n < 1E14)
        return 14;
    if (n < 1E15)
        return 15;
    if (n < 1E16)
        return 16;
    if (n < 1E17)
        return 17;
    if (n < 1E18)
        return 18;
    if (n < 1E19)
        return 19;
    if (n < 1E20)
        return 20;

    return 21;
}
uint8_t intmax_t_find_number_of_digits(intmax_t n)
{

	uint8_t s=0;//sign
    if (n < 0)
    {
        n = (n == INTMAX_MIN) ? INTMAX_MAX : -n;
        s =1;
    }
    return uintmax_t_find_number_of_digits((uintmax_t)n) + s;
}
int float_find_number_of_digits(float f, uint8_t d)//float & double
{
    //sd=sign+digits
    uint8_t sd = intmax_t_find_number_of_digits( (intmax_t) f);
    return sd + 1 + d;	//iii.dd
}
int double_find_number_of_digits(double f, uint8_t d)//float & double
{
    //sd=sign+digits
    uint8_t sd = intmax_t_find_number_of_digits( (intmax_t) f);
    return sd + 1 + d;	//iii.dd
}

int32_t get_body_length(void)
{
    char *name;
    int8_t type;
    void * addr;
    sizedata_t size;
    //
    int8_t num_regs = body_register_get_num_regs();
    int8_t reg;
    uint16_t idx;

    int32_t acc=0;

    acc+=2;//{}
    acc+= (num_regs-1);//commas between registers

    for (reg=0; reg< num_regs; reg++)
    {
        name = body_register_get_name(reg);
        type = body_register_get_type(reg);
        addr = body_register_get_addr(reg);
        size = body_register_get_size(reg);

        acc+= 2+strlen(name)+1;//"str": n_squarebrackets + strlen + colon


        if ( (type > TD_BEGIN) && (type < TD_END))
        {
             switch (type)
            {
                //signeds:
                case CHAR_TD:
                    acc+= 1 + 2;// 1character+ doublequotation
                    break;

                case INT_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (int *)addr);
                    break;

                case SHORT_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (short *)addr);
                    break;

                case LONG_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (long *)addr);
                    break;

                    #ifdef LONLONG_TD_AVAILABLE
                case LONGLONG_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (long long *)addr);
                    break;
                    #endif // LONLONG_TD_AVAILABLE

                //unsigneds:
                case UCHAR_TD://send as integer
                    acc+= 1 + 2;// 1character+ doublequotation
                    break;

                case UINT_TD:
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (unsigned int *)addr);
                    break;
                case USHORT_TD:
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (unsigned short *)addr);
                    break;

                case ULONG_TD:
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (unsigned long *)addr);
                    break;

                    #ifdef LONLONG_TD_AVAILABLE
                case ULONGLONG_TD:
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (unsigned long long *)addr);
                    break;
                    #endif
                //
                case FLOAT_TD:
                    acc+= float_find_number_of_digits( (* (float*)addr), FLOAT_N_DECIMALS);
                    break;

                case DOUBLE_TD:
                    acc+= double_find_number_of_digits( (* (double*)addr), FLOAT_N_DECIMALS);
                    break;
                //
                case INT8_T_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (int8_t *)addr);
                    break;

                case INT16_T_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (int16_t *)addr);
                    break;

                case INT32_T_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (int32_t *)addr);
                    break;
                    #ifdef INT64T_TD_AVAILABLE
                case INT64_T_TD:
                    acc+= intmax_t_find_number_of_digits((intmax_t) * (int64_t *)addr);
                    break;
                    #endif
                //
                case UINT8_T_TD:
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (uint8_t *)addr);
                    break;

                case UINT16_T_TD:
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (uint16_t *)addr);
                    break;

                case UINT32_T_TD:
                    //fast workaround 17/10/2018
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (uint32_t *)addr);
                    //acc+= int_n_digits_inc_sign((int) * (uint32_t *)addr);
                    break;
                    #ifdef INT64T_TD_AVAILABLE
                case UINT64_T_TD:
                    acc+= uintmax_t_find_number_of_digits((uintmax_t) * (uint64_t *)addr);
                    break;
                    #endif
                default:
                    break;
            }

        }
        else if (type == CSTRING_TD)
        {
            acc+= strlen((char *) addr) + 2;//strlen()+""
        }
        else if ( (type > ARRAY_TD_BEGIN) && (type < ARRAY_TD_END))
        {
            acc+=(size-1)+2;//n_commas + n_squarebrackets [,,,];

            for (idx=0; idx<size; idx++)
            {
                switch (type)
                {
                    case CHAR_ARRAY_TD://solo los caracteres printables+
                        //acc+= size + (size<<1);	//n_elements+, n_doublequotations
                        acc+= 1 + 2;	//n_elements+, n_doublequotations
                        break;
                    case INT_ARRAY_TD:
                        acc+= intmax_t_find_number_of_digits( (intmax_t) ((int *)addr)[idx]);
                        break;
                    case SHORT_ARRAY_TD:
                        acc += intmax_t_find_number_of_digits( (intmax_t) ((short *)addr)[idx]);
                        break;
                    case LONG_ARRAY_TD:
                        acc+= intmax_t_find_number_of_digits( (intmax_t) ((long *)addr)[idx]);
                        break;
                        #ifdef LONLONG_TD_AVAILABLE
                    case LONGLONG_ARRAY_TD:
                        acc+= intmax_t_find_number_of_digits( (intmax_t) ((long long *)addr)[idx]);
                        break;
                        #endif // LONLONG_TD_AVAILABLE
                    //unsigneds:
                    case UCHAR_ARRAY_TD://como un entero
                        //acc+= size + (size<<1);	//n_elements+, n_doublequotations
                        acc+= 1+2;	//n_elements+, n_doublequotations
                        break;
                    case UINT_ARRAY_TD:
                        acc+= uintmax_t_find_number_of_digits( (uintmax_t) ((unsigned int *)addr)[idx]);
                        break;
                    case USHORT_ARRAY_TD:
                        acc+= uintmax_t_find_number_of_digits( (uintmax_t) ((unsigned short *)addr)[idx]);
                        break;
                    case ULONG_ARRAY_TD:
                        acc+= uintmax_t_find_number_of_digits( (uintmax_t) ((unsigned long *)addr)[idx]);
                        break;
                        #ifdef LONLONG_TD_AVAILABLE
                    case ULONGLONG_ARRAY_TD:
                        acc += uintmax_t_find_number_of_digits( (uintmax_t) ((unsigned long long *)addr)[idx]);
                        break;
                        #endif // LONLONG_TD_AVAILABLE
                    //
                    case FLOAT_ARRAY_TD:
                        //float f=;
                        acc+= float_find_number_of_digits( ( ((float*)addr)[idx]), FLOAT_N_DECIMALS);
                        break;
                    case DOUBLE_ARRAY_TD:
                        //double d = ( ((double*)addr)[idx]);
                        acc+= double_find_number_of_digits( ( ((double*)addr)[idx]), FLOAT_N_DECIMALS);
                        break;
                    //
                    case INT8_T_ARRAY_TD:
                        acc+= intmax_t_find_number_of_digits( (intmax_t) ((int8_t *)addr)[idx]);
                        break;
                    case INT16_T_ARRAY_TD:
                        acc+= intmax_t_find_number_of_digits( (intmax_t) ((int16_t *)addr)[idx]);
                        break;
                    case INT32_T_ARRAY_TD:
                        acc+= intmax_t_find_number_of_digits( (intmax_t) ((int32_t *)addr)[idx]);
                        break;
                        #ifdef INT64T_TD_AVAILABLE
                    case INT64_T_ARRAY_TD:
                        acc+= intmax_t_find_number_of_digits( (intmax_t) ((int64_t *)addr)[idx]);
                        break;
                        #endif // INT64T_TD_AVAILABLE
                    //
                    case UINT8_T_ARRAY_TD:
                        acc+= uintmax_t_find_number_of_digits( (uintmax_t) ((uint8_t *)addr)[idx]);
                        break;
                    case UINT16_T_ARRAY_TD:
                        acc+= uintmax_t_find_number_of_digits( (uintmax_t) ((uint16_t *)addr)[idx]);
                        break;
                    case UINT32_T_ARRAY_TD:
                        acc+= uintmax_t_find_number_of_digits( (uintmax_t) ((uint32_t *)addr)[idx]);
                        break;
                        #ifdef INT64T_TD_AVAILABLE
                    case UINT64_T_ARRAY_TD:
                        acc+= uintmax_t_find_number_of_digits( (uintmax_t) ((uint64_t *)addr)[idx]);
                        break;
                        #endif // INT64T_TD_AVAILABLE
                    default:
                        break;
                }
            }
        }
    }


    return acc;
}
/*
    HTTP transaction manager:

    Exec.mode:

    WAIT_NEW_EXEC_MODE = -1,
	STOP = 0,
	RUN_ONCE,
	RUN_INTERVAL,

	Status:

	IDLE,
	RUNNING

*/


struct _http_trx_t
{
    HTTP_TRX_SET_EXEC_MODE_E exec_mode;
    int8_t status;
    unsigned long exec_interval_ms;//exec with interval
    char *rx_buffer;
};
struct _http_trx_t http_trx_t =
{
    EM_WAIT_NEW_EXEC_MODE_E,//-1
    IDLE,//0
    0,
    NULL
};
void http_trx_set_status(int8_t status)
{
    http_trx_t.status = status;
}
int8_t http_trx_get_status(void)
{
    return http_trx_t.status;
}
void http_trx_set_exec_interval_ms(unsigned long interval_ms)
{
    http_trx_t.exec_interval_ms = interval_ms;
}
unsigned long http_trx_get_exec_interval_ms(void)
{
    return http_trx_t.exec_interval_ms;
}
void http_trx_set_rx_buffer(char *rx_buffer)
{
    http_trx_t.rx_buffer = rx_buffer;
}
char *http_trx_get_rx_buffer(void)
{
    return http_trx_t.rx_buffer;
}
void http_trx_set_exec_mode(HTTP_TRX_SET_EXEC_MODE exec_mode)
{
    http_trx_t.exec_mode = exec_mode.m;
}
HTTP_TRX_SET_EXEC_MODE_E http_trx_get_exec_mode(void)
{
    return http_trx_t.exec_mode;
}

/*
        HTTP transaction: response message

I’ve had a TCPClient disconnect but still have the connected property == true. So it seams the only way to tell is if a Read from the network stream returns 0 bytes. So right now I’m assuming it is disconnected if it returns 0 bytes. I was wondering if there is a better way?
Here is documented REMARKS section for tcpClient.Connected property:
"The Connected property gets the connection state of the Client socket as of the last I/O
operation. When it returns false, the Client socket was either never connected, or is no
longer connected."
"Because the Connected property only reflects the state of the connection as of the most
recent operation, you should attempt to send or receive a message to determine the current
state. After the message send fails, this property no longer returns true. Note that this
behavior is by design. You cannot reliably test the state of the connection because, in the
time between the test and a send/receive, the connection could have been lost. Your code
should assume the socket is connected, and gracefully handle failed transmissions."
So, I thing yours 'Read' way is the best one and you may forget Connected property
alltogether!



http_trx_readbuffer_ktime: Is the k-time of CPU (and resources) assigned to read data from the buffer if available.

client_stop_in_ktime: If the Server disconnect the client, client has a k-time to read all possible available data from rx buffer. After this time,the connection must be end sending client.stop()

http_trx_response_msg_ktimeout: Is the global k-timeout assigned for all recepcion in an HTTP Transfer response-mode.

    return:
    0: Busy
    1: End response msg
*/
const unsigned long http_trx_readbuffer_ktime = 10;//ms
unsigned long client_stop_in_ktime= 700;//ms
unsigned long http_trx_response_msg_ktimeout = 1000;//ms

int8_t http_trx_response_msg(void)
{
    static int8_t sm0;
    static int8_t sm1;
    static unsigned long tmr_response_msg_timeout;
    static unsigned long tmr_client_stop;
    unsigned long tmr_readbuffer;
    int8_t cod_ret = 0;
    char c;
    static uint16_t idx;
    char *prx_buffer;
    if (sm0 == 0)
    {
        tmr_response_msg_timeout = __millis();
        idx = 0;
        sm1 = 0;
        sm0++;
    }
    if (sm0 == 1)
    {
        tmr_readbuffer = __millis();
        prx_buffer = http_trx_get_rx_buffer();
        do
        {
            if (tcp_client_available())
            {
                c = http_client_read_char();//client.read();
                nt_debug_print(c);          //Serial.print(c);

                if (prx_buffer!= NULL)
                {
                    prx_buffer[idx] = c;
                    if (++idx >= HTTP_TRX_RX_BUFFER_MAX_SIZE)
                    {
                        idx = 0;//as circular buffer
                    }
                }

                /*if (c == 'LAST_CHAR_BREAKING')
                	break;				*/
            }
            else
            {
                break;
            }

        }
        while ((__millis() - tmr_readbuffer) <= http_trx_readbuffer_ktime);  //CPU assigned for window

        if (sm1 == 0)
        {
            if (!tcp_client_connected())
            {

                if (client_stop_in_ktime > http_trx_response_msg_ktimeout)
                {
                    client_stop_in_ktime = http_trx_response_msg_ktimeout;
                }
                tmr_client_stop = __millis();
                sm1++;
            }
        }
        else//sm1 == 1
        {
            if ( (__millis() - tmr_client_stop) >= client_stop_in_ktime ) //let a time to read all rx buffer
            {
                tcp_client_stop();
                sm0 = 0;
                cod_ret = 1;
            }
        }

        //connection time-out
        if ( (__millis() - tmr_response_msg_timeout) >= http_trx_response_msg_ktimeout) //abort and stop conection
        {
            tcp_client_stop();
            sm0 = 0;
            cod_ret = 1;
        }
    }
    return cod_ret;
}

/*
                one HTTP transaction

Like most network protocols, HTTP uses the client-server model: An HTTP
client opens a connection and sends a request message to an HTTP server;
the server then returns a response message, usually containing the resource
that was requested. After delivering the response, the server closes the
connection (making HTTP a stateless protocol, i.e. not maintaining any
connection information between transactions).

    return:
    0: Busy in HTTP transaction
    1: End one HTTP transaction, even if client could not connect to the server.
*/
const PROGMEM char kstr_NL_HTTP_TRANSACTION_BEGIN_NL[]="\nHTTP transaction begin\n";
const PROGMEM char kstr_TRY_TO_CONNECT_TO_SERVER_NL[]="Try to connect to server\n";
const PROGMEM char kstr_CONNECTED_NL[]="Connected\n";
const PROGMEM char kstr_CLIENT_SERVER_SEND_REQUEST_MSG_NLNL[]="Client->server: send request-message...\n\n";
const PROGMEM char kstr_CONNECTION_FAILED_NL[]="Connection failed\n";
const PROGMEM char kstr_HTTP_TRANSACTION_END_NL[]="HTTP transaction end\n";
const PROGMEM char kstr_NL_SERVER_CLIENT_RECEIVE_RESPONSE_MSG_NLNL[]="\nServer->client: receive response-message...\n\n";
const PROGMEM char kstr_NLNL_HTTP_TRANSACTION_END_NLNL[]="\n\nHTTP transaction end\n\n";

#include "utility/w5100.h"
#include "utility/socket.h"
void ShowSocketStatus(void);

int8_t http_trx(void)
{
    static int8_t sm0;
    int8_t code_ret = 0;

    if (sm0 == 0)// client opens a connection
    {
        nt_debug_print_prgmem(kstr_NL_HTTP_TRANSACTION_BEGIN_NL);//"\nHTTP transaction begin\n"
        nt_debug_print_prgmem(kstr_TRY_TO_CONNECT_TO_SERVER_NL);//"Try to connect to server\n"
        if (tcp_client_connection())
        {
            nt_debug_print_prgmem(kstr_CONNECTED_NL);//"Connected\n"
            nt_debug_print_prgmem(kstr_CLIENT_SERVER_SEND_REQUEST_MSG_NLNL);//"Client->server: send request-message...\n\n"
            sm0++;
        }
        else
        {
            nt_debug_print_prgmem(kstr_CONNECTION_FAILED_NL);//"Connection failed\n"
            nt_debug_print_prgmem(kstr_HTTP_TRANSACTION_END_NL);//"HTTP transaction end\n"
Serial.println(F("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"));
Serial.println(F("Socket status in error before client.stop():"));
ShowSocketStatus();
   tcp_client_stop();//         client.stop();
Serial.println(F("Socket status in error after client.stop()"));
            code_ret = 1;
        }
       
    }
    if (sm0 == 1)//client->server: send request message
    {
        if (http_trx_request_msg())
        {
            sm0++;
            nt_debug_print_prgmem(kstr_NL_SERVER_CLIENT_RECEIVE_RESPONSE_MSG_NLNL);//"\nServer->client: receive response-message...\n\n"
        }
    }
    if (sm0 == 2)//server->client: receive response message
    {
        if (http_trx_response_msg())
        {
            sm0 = 0x00;
            code_ret = 1;
            nt_debug_print_prgmem(kstr_NLNL_HTTP_TRANSACTION_END_NLNL);//"\n\nHTTP transaction end\n\n"
        }
    }
Serial.println(F("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));
ShowSocketStatus();
Serial.println(F("----------------------------------------------------------------------------"));
    return code_ret;
}

/*
    return:
    0: Busy in HTTP job (synchronize RUN_ONCE, RUN_INTERVAL, STOP)
    1: End one HTTP job (end transaction): Is the time for parsing the http_trx_rx_buffer[]
*/
int8_t http_trx_job(void)
{
    static HTTP_TRX_SET_EXEC_MODE_E last_exec_mode;
    static int8_t _run_interval=0;
    static unsigned long tmr_run_interval;
    int8_t cod_ret = 0;

    if (http_trx_get_status() == IDLE)
    {
        last_exec_mode = http_trx_get_exec_mode();

        if ( last_exec_mode > EM_WAIT_NEW_EXEC_MODE_E)
        {
            if (last_exec_mode == EM_RUN_INTERVAL_E)
            {
                if (_run_interval == 0)
                {
                    http_trx_set_status(RUNNING);//first time
                    _run_interval++;	//exit and execute
                }
                else if (_run_interval == 1)//for next evaluation
                {
                    tmr_run_interval = __millis();
                    _run_interval++;
                }
                else if (_run_interval == 2)
                {
                    if ( (__millis()-tmr_run_interval) >= http_trx_get_exec_interval_ms())
                    {
                        _run_interval = 0x00;
                    }
                }
            }
            else if (last_exec_mode == EM_RUN_ONCE_E)
            {
                http_trx_set_status(RUNNING);
            }
            else//STOP
            {
                http_trx_set_exec_mode(EM_WAIT_NEW_EXEC_MODE);
                //
                _run_interval = 0x00;	//reset
            }
        }

    }
    else
    {
        if (http_trx() == 1)//end?
        {
            http_trx_set_status(IDLE);

            if (last_exec_mode == EM_RUN_ONCE_E)
            {
                http_trx_set_exec_mode(EM_STOP);
            }
            cod_ret = 1;
        }
    }
    return cod_ret;
}

//////////////////////////////////////////////////////////////////////////////////

void ShowSocketStatus(void)
{

    //    ActivityWriteSPICSC("ETHERNET SOCKET LIST");
    //    ActivityWriteSPICSC("#:Status Port Destination DPort");
    //    ActivityWriteSPICSC("0=avail,14=waiting,17=connected,22=UDP");
    //    ActivityWriteSPICSC("1C=close wait");
    String l_line = "";
    l_line.reserve(64);
    char l_buffer[10] = "";

    for (uint8_t i = 0; i < MAX_SOCK_NUM; i++)
    {
        l_line = "#" + String(i);

        uint8_t s = W5100.readSnSR(i); //status
        l_line += ":0x";
        sprintf(l_buffer,"%x",s);
        l_line += l_buffer;
        //
        // if(s == 0x1C)
        //{close(i);}
        //
        l_line += " ";
        l_line += String(W5100.readSnPORT(i)); //port

        l_line += " D:";
        uint8_t dip[4];
        W5100.readSnDIPR(i, dip); //IP Address
        for (int j=0; j<4; j++)
        {
            l_line += int(dip[j]);
            if (j<3) l_line += ".";
        }
        l_line += " (";
        l_line += String(W5100.readSnDPORT(i)); //port on destination
        l_line += ") ";

        //        if (G_SocketConnectionTimes[i] != 0)
        //            l_line += TimeToHHMM(G_SocketConnectionTimes[i]);

        Serial.println(l_line);

        //ActivityWriteSPICSC(l_line);
    }
}
