#ifndef NTSETTING_H_
#define NTSETTING_H_
//setters
void ntsetting_set_MACaddr(uint8_t *MACaddr);
void ntsetting_set_IPaddr(uint8_t *IPaddr);//Static falling back IP4
void ntsetting_DHCP_enable(void);//by default is enabled. Try to configure as dynamic again
void ntsetting_DHCP_disable(void);//try to configure as static again
void ntsetting_serialdbg_enable(void);
void ntsetting_serialdbg_disable(void);
//getters
uint8_t * ntsetting_get_MACaddr(void);
uint8_t * ntsetting_get_IPaddr(void );//Static falling back IP4
uint8_t ntsetting_DHCP_is_enabled(void);
uint8_t ntsetting_serialdbg_is_enabled(void);

#endif // NTSETTING_H_
