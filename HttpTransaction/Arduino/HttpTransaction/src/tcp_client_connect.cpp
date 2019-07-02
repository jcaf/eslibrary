
#if defined(__AVR__) && defined(__GNUC__)
    #include <Arduino.h>
    #include <Ethernet.h>

    EthernetClient client;
    #define tcp_client_connect(ip_domain,port) client.connect(ip_domain, port)
#else //__GNUC__
    #define tcp_client_connect(ip_domain,port) (1)
    #include <stdio.h>
    #include <stdint.h>
#endif // defined

#include "tcp_client_connect.h"

int8_t tcp_client_connect_by_IPaddr(void)
{
    uint8_t *IPaddr = tcp_client_get_server_IPaddr();
    int port = tcp_client_get_server_port();
    int8_t cod_ret = tcp_client_connect(IPaddr, port);
    return cod_ret;
}
int8_t tcp_client_connect_by_domain(void)
{
    char *domain = tcp_client_get_server_domain();
    int port = tcp_client_get_server_port();
    int8_t cod_ret = tcp_client_connect(domain, port);
    return cod_ret;
}
int8_t tcp_client_connection(void)
{
    int8_t cod_ret;
    char *domain;
    uint8_t  *IPaddr;
    int port;

    domain = tcp_client_get_server_domain();
    IPaddr = tcp_client_get_server_IPaddr();
    port = tcp_client_get_server_port();

    if (domain != NULL)
    {
        cod_ret = tcp_client_connect(domain, port);
    }
    else
    {
        cod_ret = tcp_client_connect(IPaddr, port);
    }
    return cod_ret;
}


