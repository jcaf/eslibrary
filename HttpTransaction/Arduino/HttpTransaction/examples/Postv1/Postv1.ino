#include <Arduino.h>
#include <http_trx.h>

void spi_deselect_devices(void);//only for QUANTICO BOARD

//local network setting
uint8_t IPaddr[4]= {192,168,1,47};//local fallback IP
uint8_t MACaddr[6]= { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Connect to server - setting
char server[] = "api.quanticoservices.net";
    
//HTTP transaction: send request message, the Body variables:
char device_id[] = "5b7f277fef4ad681b203b444";
float channel_0;
float channel_1;
float channel_2;
float channel_3;
float channel_4;
float channel_5;

//HTTP transaction: receive response message from Server (if parsing)
//#define HTTP_TRX_RX_BUFFER
#ifdef HTTP_TRX_RX_BUFFER
char http_trx_rx_buffer[HTTP_TRX_RX_BUFFER_MAX_SIZE];//as circular buffer
#endif
void setup(void)
{
    spi_deselect_devices();//QUANTICO board

    //1) local network setting
    ntsetting_serialdbg_enable();
    if (ntsetting_serialdbg_is_enabled())
    {
        nt_debug_begin(230400);//Serial hardware by default 
    }
    ntsetting_set_MACaddr(MACaddr);
    ntsetting_set_IPaddr(IPaddr);//fallback IP
    ntsetting_DHCP_enable();
    delay(1000);
    nt_debug_print("Give the Ethernet shield (W5100)a second to initialize...\n");
    ethernet_begin();

    //2) Client config connection to server
    tcp_client_set_server_domain(server);//domain priority
    //tcp_client_set_server_IPaddr(IPaddr_server);
    //tcp_client_set_server_port(80);//80 by default

    //3) Set HTTP transaction
    //3.1) set request line
    http_rqstline_set_method(POST);
    http_rqstline_set_URIpath("/devices/data/add");
    http_rqstline_set_http_version(HTTP_1_1);

    //3.2) set header lines
    http_hdrlines_set_Host("api.quanticoservices.net");
    http_hdrlines_set_ContentType(APPLICATION_JSON);
    http_hdrlines_set_Connection(CLOSE);
    http_hdrlines_set_UserAgent("Agent/1.00");
    http_hdrlines_set_ContentLength(CONTENT_LENGTH_DYNAMIC);
    
    //free header (e.g non standar http header)
    http_hdrlines_add_lines("Api-Key: 12345\
                            \r\n");
    
    //3.3) Add variables to body
    http_msgbody_add_cstring(device_id);
    http_msgbody_add_float(channel_0);
    http_msgbody_add_float(channel_1);
    http_msgbody_add_float(channel_2);
    http_msgbody_add_float(channel_3);
    http_msgbody_add_float(channel_4);
    http_msgbody_add_float(channel_5);
    
    //4) Http transaction setting
    http_trx_set_exec_interval_ms(1000);//ms
    http_trx_set_mode_exec(RUN_INTERVAL);//RUN_ONCE
    //http_trx_set_rx_buffer(http_trx_rx_buffer);//NULL pointer by defect
}
void read_channels(void)
{
  channel_0 = analogRead(0)*5.0/1023;
  channel_1 = analogRead(1)*5.0/1023;
  channel_2 = analogRead(2)*5.0/1023;
  channel_3 = analogRead(3)*5.0/1023;
  channel_4 = analogRead(4)*5.0/1023;
  channel_5 = analogRead(5)*5.0/1023;
}
void loop(void)
{
    read_channels();

    if (http_trx_job())//complete a client->server transaction, then...
    {
      //its safe to read http_trx_rx_buffer[]
        #ifdef HTTP_TRX_RX_BUFFER
          nt_debug_print("Printing http_trx_rx_buffer[]:\n\n");
          if (http_trx_get_rx_buffer()!= NULL)
          {
            for (int i=0; i<HTTP_TRX_RX_BUFFER_MAX_SIZE; i++)
              {nt_debug_print(http_trx_rx_buffer[i]);}  
          }
        #endif
    }
}

void spi_deselect_devices(void)//only for QUANTICO BOARD
{
    #ifdef Arduino_h
    #define WIZNET_CS 10
    #define SDCARD_CS 4
    #define LTC6804_CS 5
    pinMode(WIZNET_CS, OUTPUT);
    pinMode(SDCARD_CS, OUTPUT);
    pinMode(LTC6804_CS, OUTPUT);
    digitalWrite(WIZNET_CS, HIGH);
    digitalWrite(SDCARD_CS, HIGH);
    digitalWrite(LTC6804_CS, HIGH);
    #endif
}

