#ifndef HTTP_HEADER_LINES_H_
#define HTTP_HEADER_LINES_H_

#include <stdint.h>



typedef
enum _http_hdr_content_type_e
{
    APPLICATION_NONE_E=0,
    APPLICATION_X_WWW_FORM_URLENCODED_E,
    APPLICATION_JSON_E,
    APPLICATION_XML_E,
}
HTTP_HDR_CONTENT_TYPE_E;

typedef struct http_hdr_content_type_t
{
    HTTP_HDR_CONTENT_TYPE_E  ct;
} HTTP_HDR_CONTENT_TYPE;

#define APPLICATION_NONE ((HTTP_HDR_CONTENT_TYPE){APPLICATION_NONE_E})
#define APPLICATION_X_WWW_FORM_URLENCODED ((HTTP_HDR_CONTENT_TYPE){APPLICATION_X_WWW_FORM_URLENCODED_E})
#define APPLICATION_JSON ((HTTP_HDR_CONTENT_TYPE){APPLICATION_JSON_E})
#define APPLICATION_XML ((HTTP_HDR_CONTENT_TYPE){APPLICATION_XML_E})

typedef
enum _http_hdr_connection_e
{
    CONNECTION_CLOSE_E,
    CONNECTION_KEEP_ALIVE_E
}
HTTP_HDR_CONNECTION_E;

typedef struct http_hdr_connection_t
{
    HTTP_HDR_CONNECTION_E c;
} HTTP_HDR_CONNECTION;

#define CONNECTION_CLOSE ((HTTP_HDR_CONNECTION){CONNECTION_CLOSE_E})
#define CONNECTION_KEEP_ALIVE ((HTTP_HDR_CONNECTION){CONNECTION_KEEP_ALIVE_E})


//typedef
enum _http_hdr_content_length_e
{
    CONTENT_LENGTH_DYNAMIC=-1,
};
//HTTP_HDR_CONTENT_LENGTH_E;

struct _http_hdrlines
{
    //full integers
    HTTP_HDR_CONTENT_TYPE_E ContentType;
    int32_t ContentLength;
    HTTP_HDR_CONNECTION_E Connection;
    //full char *
    const char *Host;
    const char *UserAgent;
    const char *lines;//free lines
};

extern struct _http_hdrlines http_hdrlines;
//setters
void http_hdrlines_set_ContentType(HTTP_HDR_CONTENT_TYPE ContentType);
void http_hdrlines_set_ContentLength(uint16_t ContentLength);
void http_hdrlines_set_Host(const char *Host);
void http_hdrlines_set_UserAgent(const char *UserAgent);
void http_hdrlines_add_lines(const char *lines);//free header lines
void http_hdrlines_set_Connection(HTTP_HDR_CONNECTION connection);
//getters
HTTP_HDR_CONTENT_TYPE_E  http_hdrlines_get_ContentType(void);
int16_t http_hdrlines_get_ContentLength(void);
const char * http_hdrlines_get_Host(void);
const char * http_hdrlines_get_UserAgent(void);
HTTP_HDR_CONNECTION_E http_hdrlines_get_Connection(void);

#endif
