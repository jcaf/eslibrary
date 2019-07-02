#ifndef HTTP_RQSTLINE_H_INCLUDED
#define HTTP_RQSTLINE_H_INCLUDED

#include <stdint.h>
typedef
enum _http_rqstline_method_e
{
    GET_E=0,
    POST_E,
    PUT_E,
    HEAD_E,
    DELETE_E

}
HTTP_RQSTLINE_METHOD_E;
typedef struct http_rqstline_method_t
{
    HTTP_RQSTLINE_METHOD_E m;
}HTTP_RQSTLINE_METHOD;
#define GET ((HTTP_RQSTLINE_METHOD){GET_E})
#define POST ((HTTP_RQSTLINE_METHOD){POST_E})
#define PUT ((HTTP_RQSTLINE_METHOD){PUT_E})
#define HEAD ((HTTP_RQSTLINE_METHOD){HEAD_E})
#define DELETE ((HTTP_RQSTLINE_METHOD){DELETE_E})

//
typedef
enum _http_rqstline_http_version_e
{
    HTTP_1_1_E=0,//default
    HTTP_1_0_E,
}
HTTP_RQSTLINE_HTTP_VERSION_E;
typedef struct http_rqstline_http_version_t
{
    HTTP_RQSTLINE_HTTP_VERSION_E v;
}HTTP_RQSTLINE_HTTP_VERSION;
#define HTTP_1_1 ((HTTP_RQSTLINE_HTTP_VERSION){HTTP_1_1_E})
#define HTTP_1_0 ((HTTP_RQSTLINE_HTTP_VERSION){HTTP_1_0_E})
//
void http_rqstline_set_method(HTTP_RQSTLINE_METHOD method);
void http_rqstline_set_URIpath(const char *URIpath);
void http_rqstline_set_http_version(HTTP_RQSTLINE_HTTP_VERSION http_version);

HTTP_RQSTLINE_METHOD_E http_rqstline_get_method(void);
const char * http_rqstline_get_URIpath(void);
HTTP_RQSTLINE_HTTP_VERSION_E http_rqstline_get_http_version(void);

#endif // HTTP_RQSTLINE_H_INCLUDED
