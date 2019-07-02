#if defined(__AVR__) && defined(__GNUC__)
#include <Arduino.h>
#include <Ethernet.h>
#endif // defined

#include <stdint.h>
#include "ethernet.h"
#include "iot/ntsetting.h"

const PROGMEM char kstr_MY_LOCAL_IP_ADDRESS_IS[]="My local IP address is: ";
const PROGMEM char kstr_NLNL[]="\n\n";

void nt_print_IP(void)
{
	nt_debug_print_prgmem(kstr_MY_LOCAL_IP_ADDRESS_IS);//"My local IP address is: "
	nt_debug_print(Ethernet.localIP());
	nt_debug_print_prgmem(kstr_NLNL);//"\n\n"
}

const PROGMEM char kstr_TRY_TO_CONFIGURE_USING_DHCP_NL[]="Try to configure using DHCP...\n";
const PROGMEM char kstr_TRYING_TO_CONFIGURE_USING_IP_ADDRES_INSTEAD_OF_DHCP_NL[]="Trying to configure using IP address instead of DHCP...\n";
const PROGMEM char kstr_STATIC_IP_WAS_ASSIGNED_NL[]="Static IP was assigned\n";
const PROGMEM char kstr_IP_CONFIGURED_BY_DHCP_NL[]="IP configured by DHCP\n";


int8_t ethernet_begin(void)//start the Ethernet connection
{
    int8_t cod_ret=1;//Only begin(mac) return a value, others overloaded fxs return void
    #if defined(__AVR__) && defined(__GNUC__)
    uint8_t *mac = ntsetting_get_MACaddr();
    IPAddress ip(ntsetting_get_IPaddr());

    if (ntsetting_DHCP_is_enabled())
    {
        nt_debug_print_prgmem(kstr_TRY_TO_CONFIGURE_USING_DHCP_NL);//"Try to configure using DHCP...\n"
        if (Ethernet.begin(mac) == 0)
        {
            nt_debug_print_prgmem(kstr_TRYING_TO_CONFIGURE_USING_IP_ADDRES_INSTEAD_OF_DHCP_NL);//"Try to configure using IP address instead of DHCP...\n"
            Ethernet.begin(mac, ip);
            nt_debug_print_prgmem(kstr_STATIC_IP_WAS_ASSIGNED_NL);//"Static IP was assigned\n"
            cod_ret=2;
        }
        else
        {
            nt_debug_print_prgmem(kstr_IP_CONFIGURED_BY_DHCP_NL);//"IP configured by DHCP\n"
        }
    }
    else
    {
        nt_debug_print_prgmem(kstr_TRYING_TO_CONFIGURE_USING_IP_ADDRES_INSTEAD_OF_DHCP_NL);//"Try to configure using IP address instead of DHCP\n"
        Ethernet.begin(mac, ip);//others overloaded fxs return void
        nt_debug_print_prgmem(kstr_STATIC_IP_WAS_ASSIGNED_NL);//"Static IP was assigned\n"
        cod_ret=2;
    }

    nt_print_IP();

    #endif // defined
    return cod_ret;
}


void nt_debug_begin(long baud)
{
    #ifdef Arduino_h
    Serial.begin(baud);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    #endif // Arduino_h
}
