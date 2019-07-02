/******************************************************************************************
All theory from:

"HTTP made really easy"
"Clinton Wong - HTTP Pocket Reference (2000, O'Reilly Media)"


HTTP->take place through Tcp ip sockets
Client make request/Server make response
por este lado entonces podria distinguir de cliente y servidor para las funciones
http_rqst_headr_set()/http_rspe_set()

Uniform Resource Locator URL can be a file etc etc

1- an initial line,
2- zero or more header lines,
3- a blank line (i.e. a CRLF by itself), and
4- an optional message body (e.g. a file, or query data, or query output).

******************************************************************************************/
#include <stdio.h>
#include "http.h"
/******************************************************************************************
1) HTTP REQUEST LINE

Initial request line: has three parts separated by spaces
	1) method name
	2) local path of the request resource
	3) VERSION_OF_HTTP

	GET /path/to/file/index.html HTTP/1.0
******************************************************************************************/
struct _http_rqstline
{
    HTTP_RQSTLINE_METHOD_E rqst_method;
    const char *URIpath;
    int8_t http_version;
};
struct _http_rqstline http_rqstline = {GET_E, "/", HTTP_1_1_E};

//setters
void http_rqstline_set_method(HTTP_RQSTLINE_METHOD method)
{
    http_rqstline.rqst_method = method.m;
}
void http_rqstline_set_URIpath(const char *URIpath)
{
    http_rqstline.URIpath = URIpath;
}
void http_rqstline_set_http_version(HTTP_RQSTLINE_HTTP_VERSION http_version)
{
    http_rqstline.http_version = http_version.v;
}
//getters
//int8_t
HTTP_RQSTLINE_METHOD_E http_rqstline_get_method(void)
{
    return http_rqstline.rqst_method;
}
const char * http_rqstline_get_URIpath(void)
{
    return http_rqstline.URIpath;
}
HTTP_RQSTLINE_HTTP_VERSION_E http_rqstline_get_http_version(void)
{
    return http_rqstline.http_version;
}
/******************************************************************************************
2) HTTP HEADERS LINES

-general-header (section 4.5)
-request-header (section 5.3)
-response-header (section 6.2)
-entity-header (section 7.1)
******************************************************************************************/

struct _http_hdrlines http_hdrlines = {
    APPLICATION_NONE_E,//ContentType
    0,//ContentLength
    CONNECTION_CLOSE_E,//Connection
    NULL,//char *Host
    NULL,//char *UserAgent
    NULL,//char *lines
};
//setters
void http_hdrlines_set_ContentType(HTTP_HDR_CONTENT_TYPE ContentType)
{
    http_hdrlines.ContentType = ContentType.ct;
}
void http_hdrlines_set_ContentLength(uint16_t ContentLength)
{
    http_hdrlines.ContentLength= ContentLength;
}
void http_hdrlines_set_Host(const char *Host)
{
    http_hdrlines.Host = Host;
}
void http_hdrlines_set_UserAgent(const char *UserAgent)
{
    http_hdrlines.UserAgent = UserAgent;
}
/* This is the recommended format
http_hdrlines_add_lines("Api_Key_Write: 5b74e08....bb1e6\
                        \r\nDevice_id: 5b74e08....bb1e6\
                        \r\nXYZ: 5b74e08....bb1e6\
                        \r\n");
*/
void http_hdrlines_add_lines(const char *lines)//free header lines
{
    http_hdrlines.lines = lines;
}
void http_hdrlines_set_Connection(HTTP_HDR_CONNECTION connection)
{
    http_hdrlines.Connection = connection.c;
}

//getters
HTTP_HDR_CONTENT_TYPE_E http_hdrlines_get_ContentType(void)
{
    return http_hdrlines.ContentType;
}
int16_t http_hdrlines_get_ContentLength(void)
{
    return http_hdrlines.ContentLength;
}
const char * http_hdrlines_get_Host(void)
{
    return http_hdrlines.Host;
}
const char * http_hdrlines_get_UserAgent(void)
{
    return http_hdrlines.UserAgent;
}
HTTP_HDR_CONNECTION_E http_hdrlines_get_Connection(void)
{
    return http_hdrlines.Connection;
}


/******************************************************************************************
3) HTTP MESSAGE BODY
******************************************************************************************/
struct http_msgbody_register_t body_register[REG_NUMMAX];

struct reg_t
{
    int8_t idx;
};
static struct reg_t reg;


/*Add data to new register*/
void http_rqstmsg_add_register(const char *name, void * addr, uint8_t type, sizedata_t size)
{
	//
    int i;//truncate name to ASCIIZ
	for (i=0; i< REG_NAME_SIZE-1; i++)
        {body_register[reg.idx].name[i] = name[i];}
    body_register[reg.idx].name[i] ='\0';
	//
    body_register[reg.idx].addr= addr;
    body_register[reg.idx].type= type;
    body_register[reg.idx].size= size;

    if (++reg.idx == REG_NUMMAX)
        {reg.idx = 0;}
}
void body_register_set_NameData(const char *nameData)//optional
{

}

int8_t body_register_get_num_regs(void)
{
    return reg.idx;
}

void body_register_set_num_regs(int8_t idx)
{
    reg.idx = idx;
}
//

char* body_register_get_name(int8_t reg)
{
    return body_register[reg].name;
}
void* body_register_get_addr(int8_t reg)
{
    return body_register[reg].addr;
}
int8_t body_register_get_type(int8_t reg)
{
    return body_register[reg].type;
}
sizedata_t body_register_get_size(int8_t reg)
{
    return body_register[reg].size;
}

////uso
//char c='d';
//int int01 = -23;
//char texto01[]="Battery System";
//
//REST_body_add_Char(c);
//REST_body_add_Int(int01);
//REST_body_add_Cstring(texto01);
////

void http_msgbody_remove_all(void)//set internal reg.idx = 0
{
    body_register_set_num_regs(0);
}

