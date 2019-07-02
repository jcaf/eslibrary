#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_
    #include <stdint.h>
    void tcp_client_set_server_IPaddr(uint8_t *IPaddr);
    void tcp_client_set_server_port(int port);
    void tcp_client_set_server_domain(char *domain);

    uint8_t * tcp_client_get_server_IPaddr(void);
    int tcp_client_get_server_port(void);
    char * tcp_client_get_server_domain(void);


#endif // TCP_CLIENT_H_
