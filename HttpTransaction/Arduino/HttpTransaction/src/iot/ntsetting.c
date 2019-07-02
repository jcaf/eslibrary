#include <stdint.h>

struct _ntsetting
{
    uint8_t *MACaddr;//[6];
    uint8_t *IPaddr;//[4];
    struct _ntsetting_bf
    {
       unsigned DHCP:1;
       unsigned serialdbg:1;
       unsigned __a:6;
    }bf;
};

struct _ntsetting ntsetting;
//setters
void ntsetting_set_MACaddr(uint8_t *MACaddr)
{
    ntsetting.MACaddr = MACaddr;
}
void ntsetting_set_IPaddr(uint8_t *IPaddr)//Static falling back IP4
{
    ntsetting.IPaddr = IPaddr;
}

void ntsetting_DHCP_enable(void)//by default is enabled. Try to configure as dynamic again
{
    ntsetting.bf.DHCP = 1;
}
void ntsetting_DHCP_disable(void)//try to configure as static again
{
    ntsetting.bf.DHCP = 0;
}
void ntsetting_serialdbg_enable(void)
{
    ntsetting.bf.serialdbg = 1;
}
void ntsetting_serialdbg_disable(void)
{
    ntsetting.bf.serialdbg= 0;
}
//getters
uint8_t * ntsetting_get_MACaddr(void)
{
    return ntsetting.MACaddr;
}
uint8_t *ntsetting_get_IPaddr(void )//Static falling back IP4
{
    return ntsetting.IPaddr;
}
uint8_t ntsetting_DHCP_is_enabled(void)
{
    return ntsetting.bf.DHCP;
}
uint8_t ntsetting_serialdbg_is_enabled(void)
{
    return ntsetting.bf.serialdbg;
}
///////////////////////////////////////////
