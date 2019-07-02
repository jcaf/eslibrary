#ifndef TCP_CLIENT_CONNECT_H_//C++ header
#define TCP_CLIENT_CONNECT_H_

    extern "C"{
    #include "iot/tcp_client.h"
    }
    int8_t tcp_client_connect_by_IPaddr(void);
    int8_t tcp_client_connect_by_domain(void);
    int8_t tcp_client_connection(void);
    
    #if defined(__AVR__) && defined(__GNUC__)
      #include <Arduino.h>
      #ifdef Arduino_h
        #include <Ethernet.h>
        extern EthernetClient client;
      #endif // defined
   #endif

#endif // TCP_CLIENT_CONNECT_H_
