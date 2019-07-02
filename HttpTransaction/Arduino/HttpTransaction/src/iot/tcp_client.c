#include "tcp_client.h"
#include <stdio.h>
struct _tcp_client
{
    uint8_t *IPaddr;
    int port;
    char *domain;
}tcp_client = {NULL, 80, NULL};

//setters
void tcp_client_set_server_IPaddr(uint8_t *IPaddr)
{
    tcp_client.IPaddr = IPaddr;
}
void tcp_client_set_server_port(int port)
{
    tcp_client.port = port;
}
void tcp_client_set_server_domain(char *domain)
{
    tcp_client.domain = domain;
}

//getters
uint8_t * tcp_client_get_server_IPaddr(void)
{
    return tcp_client.IPaddr;
}
int tcp_client_get_server_port( void)
{
    return tcp_client.port;
}
char * tcp_client_get_server_domain(void )
{
    return tcp_client.domain;
}

//    enum tcp_client_method_connect_
//    {
//        IPADDR_CONNECT,
//        DOMAIN_CONNECT
//    };
//    tcp_client_connect_method(IPADDR_CONNECT);
