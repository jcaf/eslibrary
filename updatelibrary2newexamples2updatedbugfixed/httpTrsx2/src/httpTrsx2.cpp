#if defined(__AVR__) && defined(__GNUC__)
    #include <Arduino.h>
    #include <Ethernet.h>
    #define __millis() millis()
#else
#endif

#include <string.h>
#include <stdint.h>
#include "httpTrsx2.h"

#if defined(__AVR__) && defined(__GNUC__)
    void httpTrsx_setClient(TRSX *trsx, Client* client)
    {
        trsx->client = client;
    }
#else
#endif

#ifdef HTTPTRSX_DEBUG

static HTTPTRSXDEBUG httpTrsxDebug;

enum _UART_PRINT_MODE
{
	STR_RAM = 0,
	STR_FLASH
};
#if defined(__AVR__) && defined(__GNUC__)

		void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print)
		{
			httpTrsxDebug.UART_print = UART_print;
		}
		void httpTrsx_UARTdebug_setPrintlnFx(PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_println)
		{
			httpTrsxDebug.UART_println = UART_println;
		}
	#else
void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR UART_print)
{
	httpTrsxDebug.UART_print = UART_print;
}
#endif

void httpTrsx_UARTdebug_setPrintCharFx(PTRFX_retVOID_arg1_CHAR fx)
{
	httpTrsxDebug.UART_printChar = fx;
}
//
void httpTrsx_UARTdebug_enabled(TRSX *trsx, BOOLEAN_T _bool)
{
	trsx->dbg.bf.enabled = _bool.k;
}
void httpTrsx_UARTdebug_print(TRSX *trsx, const char *str, int8_t mode)
{
	if (trsx->dbg.bf.enabled)
	{
		httpTrsxDebug.UART_print((char *)str, mode);
	}
}
void httpTrsx_UARTdebug_println(TRSX *trsx, const char *str, int8_t mode)
{
	if (trsx->dbg.bf.enabled)
	{
		httpTrsxDebug.UART_println((char *)str, mode);
	}
}
void httpTrsx_UARTdebug_printChar(TRSX *trsx, char c)
{
	if (trsx->dbg.bf.enabled)
	{
		httpTrsxDebug.UART_printChar(c);
	}
}
#endif
/******************************************************************************************/
/*Print standard (RAM)*/
static int8_t http_print(TRSX *trsx, const char *s)
{
	int8_t cod_ret = 1;
#if defined(__AVR__) && defined(__GNUC__)
        //trsx->client->print(s);
		trsx->client->write((const uint8_t *)s,strlen(s));
        #ifdef HTTPTRSX_DEBUG
            httpTrsx_UARTdebug_print(trsx, s, 0);
        #endif
    #else

#endif
	return cod_ret;
}
/******************************************************************************************/
static int8_t http_printk(TRSX *trsx, char *s)
{
	int8_t cod_ret = 1;

#ifdef FS_STRING

		//trsx->client->print(reinterpret_cast <const __FlashStringHelper *> (s) );

		//Method 2
		char buff[BUFF_SIZEMAX];
		if ( strlcpy_P(buff, s, sizeof(buff)) >= sizeof(buff) )
		{
			cod_ret = 0;//error
		}
		http_print(trsx, buff);//1 msg per TCP frame

		/*//Method 1
		char c;
		char *ps = s;
		while (1)
		{
			c = pgm_read_byte_near(ps++);
			if (c == '\0')
				break;
			else
				trsx->client->write(c);//1 byte per TCP frame
			//s++;
		}
		*/
		#ifdef HTTPTRSX_DEBUG
            //httpTrsx_UARTdebug_print(trsx, s, 1);
            //httpTrsx_UARTdebug_print(trsx, buff, 0);
        #endif
    #else
	cod_ret = http_print(trsx, s);
#endif

	return cod_ret;
}
/******************************************************************************************/

/* 1 = configured
 * 0 = not configured */
//int8_t NIC_begin(uint8_t *MAC, ENABLED_T DHCPenabled , uint8_t *IPstatic)
#if defined(__AVR__) && defined(__GNUC__)

	int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic)
	{
		int8_t cod_ret = 0;

		IPAddress IP(IPstatic);
		if (1)//(DHCPenabled)
		{
			/* Only begin(mac) return a value, others overloaded fxs return void */
			if (Ethernet.begin(MAC) == 0)
			{
				Ethernet.begin(MAC, IP);//"Static IP was assigned\n"
			}
			else//"IP configured by DHCP\n"
			{
				//httpTrsxWrite_UARTdebug_print("IP configured by DHCP\n");
			}
		}
		else
		{
			Ethernet.begin(MAC, IP);//others overloaded fxs return void
		}
		cod_ret = 1;
		return cod_ret;
	}
	void NIC_getMyIP(char *str, size_t sizebuff)
	{
		snprintf(str, sizebuff, "%d.%d.%d.%d", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
	}
#else
	int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic)
	{}
	void NIC_getMyIP(char *str, size_t sizebuff)
	{}
#endif

/******************************************************************************************/
void httpTrsx_setupServerByDomain(TRSX *trsx, char *domain, uint16_t port)
{
	trsx->domain = domain;
	trsx->port = port;
}
//void httpTrsx_setupServerByIP(TRSX *trsx, uint8_t *IP, uint16_t port)
void httpTrsx_setupServerByIP(TRSX *trsx, IPAddress IP, uint16_t port)
{
	trsx->IP = IP;
	trsx->port = port;
}
static inline  int8_t tcpClient_connect(TRSX *trsx)
{
	int8_t cod_ret;
#if defined(__AVR__) && defined(__GNUC__)
		if (trsx->domain != NULL)
			cod_ret = trsx->client->connect(trsx->domain, trsx->port);
		else
			cod_ret = trsx->client->connect(trsx->IP, trsx->port);
	#else
#endif
	return cod_ret;
}

static inline int16_t tcpClient_getBytesAvailable(TRSX *trsx)
{
#if defined(__AVR__) && defined(__GNUC__)
    	return trsx->client->available();
    #else
#endif
}
static inline int8_t tcpClient_connected(TRSX *trsx)
{
#if defined(__AVR__) && defined(__GNUC__)
    	return trsx->client->connected();
    #else
#endif
}
static inline void tcpClient_flush(TRSX *trsx)
{
#if defined(__AVR__) && defined(__GNUC__)
    	trsx->client->flush();
    #else
#endif
}
static inline void tcpClient_stop(TRSX *trsx)
{
#if defined(__AVR__) && defined(__GNUC__)
    	trsx->client->stop();
    #else
#endif
}
static inline char httpClient_readChar(TRSX *trsx)
{
#if defined(__AVR__) && defined(__GNUC__)
    	return trsx->client->read();
    #else
#endif
}
/******************************************************************************************/
void httpTrsx_setHost(TRSX *trsx, const char *host)
{
	trsx->Host = host;
}
void httpTrsx_setURI(TRSX *trsx, const char *URI)
{
	trsx->URI = URI;
}
//void httpTrsx_setApiKey(TRSX *trsx, char *ApiKey)
//{
//    trsx->ApiKey = ApiKey;
//}
void httpTrsx_setHdrLine(TRSX *trsx, const char *HdrLine)
{
	trsx->HdrLine = HdrLine;
}
/******************************************************************************************/
static int8_t httpTrsx_requestMsg(TRSX *trsx, uint16_t contentLength)
{
	int8_t cod_ret = 0;
	char buff[20];

	/*1) Request Line*/
	http_printk(trsx, FS("POST "));
	http_print(trsx, trsx->URI);
	http_printk(trsx, FS(" HTTP/1.1\r\n"));

	/*2) Header lines*/
	http_printk(trsx, FS("Host: "));
	http_print(trsx, trsx->Host);
	http_printk(trsx, FS("\r\n"));
	//http_printk(trsx, FS("Connection: keep-alive\r\n"));//HTTP persistent connection
	http_printk(trsx, FS("Connection: close\r\n"));
	http_printk(trsx, FS("Content-Type: application/json\r\n"));
	http_printk(trsx, FS("User-Agent: Agent/1.00\r\n"));
	//    if (trsx->ApiKey != NULL)
	//    {
	//    	http_printk(trsx, FS("api_key: "));http_print(trsx, trsx->ApiKey);http_printk(trsx, FS("\r\n"));
	//    }
	if (trsx->HdrLine != NULL)
	{
		http_print(trsx, trsx->HdrLine);
		http_printk(trsx, FS("\r\n"));
	}

	http_printk(trsx, FS("Content-Length: "));
	cUint32ToStr(contentLength, buff, sizeof(buff));
	http_print(trsx, buff);
	http_printk(trsx, FS("\r\n"));

	/*3) Send New Line*/
	http_printk(trsx, FS("\r\n"));

	cod_ret = 1;

	return cod_ret;
}
static int8_t httpTrsx_requestMsg_asJSON(TRSX *trsx, JSON *json, uint16_t npairs)	//send the request message to HTTP server
{
	int8_t cod_ret = 0;

	if ((npairs > 0) && (json != NULL))
	{
		httpTrsx_requestMsg(trsx, json_getContentLength(json, npairs));

		/*4) Send msg body*/
		uint16_t i;
		http_printk(trsx, FS("{"));
		for (i = 0; i < npairs; i++)
		{
			http_printk(trsx, FS("\""));
			http_print(trsx, json[i].name);
			http_printk(trsx, FS("\":"));
			http_print(trsx, json[i].strval);

			if (i < (npairs - 1))
			{
				http_printk(trsx, FS(","));
			}
		}
		http_printk(trsx, FS("}"));
		//
	}
	else
	{
		httpTrsx_requestMsg(trsx, 0);
	}

	http_printk(trsx, FS("\r\n"));
	cod_ret = 1;
	return cod_ret;
}
static int8_t httpTrsx_requestMsg_asCstr(TRSX *trsx, char *cstr, uint16_t cstrLength)	//send the request message to HTTP server
{
	int8_t cod_ret = 0;

	httpTrsx_requestMsg(trsx, cstrLength);

	/*4) Send msg body*/
	if ((cstrLength > 0) && (cstr != NULL))
	{
		http_print(trsx, cstr);
	}

	//
	http_printk(trsx, FS("\r\n"));
	cod_ret = 1;
	return cod_ret;
}

/******************************************************************************************
 HTTP transaction: response message

 I’ve had a TCPClient disconnect but still have the connected property == true.
 So it seams the only way to tell is if a Read from the network stream returns 0 bytes.
 So right now I’m assuming it is disconnected if it returns 0 bytes. I was wondering if there is a better way?
 
 Here is documented REMARKS section for tcpClient.Connected property:
 "The Connected property gets the connection state of the Client socket as of the last I/O
 operation. When it returns false, the Client socket was either never connected, or is no
 longer connected."
 "Because the Connected property only reflects the state of the connection as of the most
 recent operation, you should attempt to send or receive a message to determine the current
 state. After the message send fails, this property no longer returns true. Note that this
 behavior is by design. You cannot reliably test the state of the connection because, in the
 time between the test and a send/receive, the connection could have been lost. Your code
 should assume the socket is connected, and gracefully handle failed transmissions."
 So, I thing yours 'Read' way is the best one and you may forget Connected property
 alltogether!

 Arduino:
 connected()
 Description
 Whether or not the client is connected. Note that a client is considered connected if the connection has been closed
 but there is still unread data.

 KTIMEOUT_READBUFFER: Is the k-time of CPU (and resources) assigned to read data from the buffer if available.
 KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER: If the Server disconnect the client, client has a k-time to read all possible available data from rx buffer. After this time,the connection must be end sending client.stop()
 KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT: Is the global k-timeout assigned for all recepcion in an HTTP Transfer response-mode.
 return:
 0: Busy
 1: End response msg
 *******************************************************************************************/
//const unsigned long KTIMEOUT_READBUFFER = 10;//ms
//unsigned long KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER= 700;//ms
//unsigned long KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT = 2000;//ms
//struct _httpTrsx_responseMsg_ktimeOut
//{
//}ktimeOut;
#define KTIMEOUT_READBUFFER                             60UL//ms
#define KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER    (5)//ms
#define KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT               10000UL//ms

#if KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER > KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT
    #undef KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER
    #define KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT
#endif // KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER

//1=finish
//2=outmsg is already to parse
//int8_t httpTrsx_responseMsg(TRSX *trsx, char *rxmsg, size_t rxmsgSize)
int8_t httpTrsx_responseMsg(TRSX *trsx, char *rxmsg, size_t rxmsgSize, uint16_t *rxCounterChar)
{
	unsigned long tmr_readbuffer;
	char c;
	size_t idx = 0;
	int8_t cod_ret = 0;

	//------------------------
#ifdef HTTPTRSX_DEBUG
	char buff[20];
	static int n;
	n++;
	strcpy(buff, "\nbuff: ");
	cIntToStr(n, &buff[strlen(buff)], 20-strlen(buff));
	httpTrsx_UARTdebug_println(trsx, buff, STR_RAM);
#endif
	//------------------------

	if (trsx->respMsg.sm0 == 0)
	{
		trsx->respMsg.timer.responseMsg_totalTimeout = __millis();
		trsx->respMsg.sm1 = 0;
		trsx->respMsg.sm0++;

		//++--
		unsigned long m = millis();
		int sk = 0;
		while ((millis() - m) < 10000UL)
		{
			if (sk == 0)
			{
				if (httpClient_readChar(trsx) == '\r')
					sk++;
			}
			if (sk == 1)
			{
				if (httpClient_readChar(trsx) == '\n')
					sk++;
				else
					sk = 0;
			}
			if (sk == 2)
			{
				if (httpClient_readChar(trsx) == '\r')
					sk++;
				else
					sk = 0;
			}
			if (sk == 3)
			{
				if (httpClient_readChar(trsx) == '\n')
				{
					sk++;
					break;
				}
				else
					sk = 0;
			}
		}
		//--+
	}
	if (trsx->respMsg.sm0 == 1)
	{
		#ifdef HTTPTRSX_DEBUG
		httpTrsx_UARTdebug_print(trsx, FS("en trsx->respMsg.sm0 == 1"), 1);
		#endif

		tmr_readbuffer = __millis();
		idx = 0;

		//Serial.print("library Serial.Write-->>");

		do
		{
			if (tcpClient_getBytesAvailable(trsx) > 0)	//buffer>0
			{
				c = httpClient_readChar(trsx);

				if (rxCounterChar != (uint16_t*)NULL)
						{*rxCounterChar+=1;}

				#ifdef HTTPTRSX_DEBUG
				httpTrsx_UARTdebug_printChar(trsx, c);
				#endif

				if (rxmsg != NULL)
				{
					rxmsg[idx] = c;
					if (++idx >= rxmsgSize)	//as circular buffer
					{
						idx = 0;
					}
					//
					//cod_ret = 2;
					//Serial.write(c);
				}
			}
			else
			{
				//Serial.println("\ntermino en break");
				break;
			}
		} while ((__millis() - tmr_readbuffer) <= KTIMEOUT_READBUFFER); //CPU assigned for window

		//Serial.println("\ncod_ret:" + String(cod_ret));


		if (trsx->respMsg.sm1 == 0)
		{
			if (!tcpClient_connected(trsx))
			{
				trsx->respMsg.timer.afterServerDisconneted_flushbuffer = __millis();
				trsx->respMsg.sm1++;
			}
		}
		else
		{
			if ((__millis() - trsx->respMsg.timer.afterServerDisconneted_flushbuffer) >= KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER) //let a time to read all rx buffer
			{
				tcpClient_flush(trsx);
				tcpClient_stop(trsx);
				trsx->respMsg.sm0 = 0;
				cod_ret = 1;
			}
		}

		//connection time-out
		if ((__millis() - trsx->respMsg.timer.responseMsg_totalTimeout) >= KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT) //abort and stop conection
		{
			tcpClient_flush(trsx);
			tcpClient_stop(trsx);
			trsx->respMsg.sm0 = 0;
			cod_ret = 1;
		}
	}
	#ifdef HTTPTRSX_DEBUG
	if (cod_ret == 1)
	{
		n = 0;
	}
	#endif

	return cod_ret;
}

/*
 1 HTTP transaction

 Like most network protocols, HTTP uses the client-server model: An HTTP
 client opens a connection and sends a request message to an HTTP server;
 the server then returns a response message, usually containing the resource
 that was requested. After delivering the response, the server closes the
 connection (making HTTP a stateless protocol, i.e. not maintaining any
 connection information between transactions).

 return:
 0: Busy in HTTP transaction
 1: End one HTTP transaction, even if client could not connect to the server.
 */

#ifdef SOCKET_DEBUG
void ShowSocketStatus(void);
#endif
//1=finish
//2=outmsg is already to parse
//int8_t httpTrsx_do1trsx(TRSX *trsx, JSON *json, uint8_t npairs, char *outmsg, uint16_t outmsgSize)
int8_t httpTrsx_do1trsx(TRSX *trsx, int8_t typeData, void *txmsg, uint16_t txmsgNumMax, char *rxmsg, size_t rxmsgSize, uint16_t *rxCounterChar)
{
	int8_t cod_ret = 0;

	if (trsx->do1trsx.sm0 == 0) // client opens a connection
	{
		#ifdef HTTPTRSX_DEBUG
		httpTrsx_UARTdebug_print(trsx, FS("\ntcpClient_connect(trsx)"), 1);
		#endif

		int tcpClient_connect_Rpta = tcpClient_connect(trsx);

		//++--
		#ifdef HTTPTRSX_DEBUG
			char buff[40];
			strcpy(buff, "\ntcpClient_connect_Rpta: ");
			cIntToStr(tcpClient_connect_Rpta, &buff[strlen(buff)], 20-strlen(buff));
			httpTrsx_UARTdebug_println(trsx, buff, STR_RAM);
		#endif

		//if (tcpClient_connect(trsx))
		if (tcpClient_connect_Rpta == 1)
		{
			trsx->do1trsx.sm0++;
		}
		else
		{
			#ifdef HTTPTRSX_DEBUG
				httpTrsx_UARTdebug_print(trsx, FS("\nNO CONECTO:ShowSocketStatus()"), STR_FLASH);

				#ifdef SOCKET_DEBUG
				ShowSocketStatus();
				#endif
			#endif

			cod_ret = -1;
		}
	}
	if (trsx->do1trsx.sm0 == 1) //client->server: send request message
	{

		if (typeData == TX_JSONSTRUCT)
		{
			if (httpTrsx_requestMsg_asJSON(trsx, (JSON*) txmsg, txmsgNumMax))
			{
				trsx->do1trsx.sm0++;
			}
		}
		else
		{
			if (httpTrsx_requestMsg_asCstr(trsx, (char*) txmsg, txmsgNumMax))
			{
				#ifdef HTTPTRSX_DEBUG
					httpTrsx_UARTdebug_print(trsx, FS("\nCstr:"), STR_FLASH);
					httpTrsx_UARTdebug_println(trsx, (char*) txmsg, STR_RAM);
				#endif

				trsx->do1trsx.sm0++;
			}
		}
	}
	if (trsx->do1trsx.sm0 == 2)        //server->client: receive response message
	{
		cod_ret = httpTrsx_responseMsg(trsx, rxmsg, rxmsgSize, rxCounterChar);
		//1=finish
		//2=outmsg is already to parse -> deprecated
		if (cod_ret == 1)
		{
			trsx->do1trsx.sm0 = 0x00;
		}
	}

	#ifdef SOCKET_DEBUG
    ShowSocketStatus();
    #endif

	return cod_ret;
}

int8_t httpTrsx_getStatus(TRSX *trsx)
{
	return trsx->exec.status;
}
void httpTrsx_setExecIntervalRepetitive_ms(TRSX *trsx, unsigned long interval_ms)
{
	if (interval_ms < HTTPTRSX_MIN_INTERVAL_MS)//500ms
		trsx->exec.execInterval_ms = HTTPTRSX_MIN_INTERVAL_MS;
	else
		trsx->exec.execInterval_ms = interval_ms;
}
void httpTrsx_setExecMode(TRSX *trsx, HTTP_TRSX_SET_EXEC_MODE execMode)
{
	trsx->exec.execMode = execMode.k;
}
/*
 return:
 0: Busy in HTTP job (synchronize RUN_ONCE, RUN_INTERVAL, STOP)
 1: End one HTTP job (end transaction): Is the time for parsing the http_trx_rx_buffer[]
 -1: Transaction has been completed with ERRORS: failed a Conexion, Server dropped, etc etc
 */
int8_t httpTrsx_job(TRSX *trsx, int8_t typeData, void *txmsg, uint16_t txmsgNumMax, char *rxmsg, size_t rxmsgSize, uint16_t *rxCounterChar)
{
	int8_t cod_ret = 0;

	if (trsx->exec.status == IDLE)
	{
		trsx->exec.last_execMode = trsx->exec.execMode;

		if (trsx->exec.last_execMode > EM_WAIT_NEW_EXEC_MODE_E)
		{
			if (trsx->exec.last_execMode == EM_RUN_REPETITIVE_E)
			{
				if (trsx->exec.runInterval_sm0 == 0)
				{
					trsx->exec.status = RUNNING;
					trsx->exec.runInterval_sm0++;
					/*execute 1 HTTP transaction, then comeback to here to continue with runInterval_sm0=1*/
				}
				else if (trsx->exec.runInterval_sm0 == 1)		//for next evaluation
				{
					trsx->exec.tmr_runInterval = __millis();
					trsx->exec.runInterval_sm0++;
				}
				else if (trsx->exec.runInterval_sm0 == 2)
				{
					if ((__millis() - trsx->exec.tmr_runInterval) >= trsx->exec.execInterval_ms)
					{
						trsx->exec.runInterval_sm0 = 0x00;
					}
				}
			}
			else if (trsx->exec.last_execMode == EM_RUN_ONCE_E)
			{
				trsx->exec.status = RUNNING;
			}
			else		//STOP
			{
				httpTrsx_setExecMode(trsx, EM_WAIT_NEW_EXEC_MODE);
				trsx->exec.runInterval_sm0 = 0x00;	//reset
			}
		}
	}
	//else
	if (trsx->exec.status == RUNNING)
	{
		cod_ret = httpTrsx_do1trsx(trsx, typeData, txmsg, txmsgNumMax, rxmsg, rxmsgSize, rxCounterChar);
		if ((cod_ret == 1) || (cod_ret == -1))
		{
			trsx->exec.status = IDLE;

			if (trsx->exec.last_execMode == EM_RUN_ONCE_E)
			{
				httpTrsx_setExecMode(trsx, EM_STOP);
			}
		}
	}
	return cod_ret;
}
/* */
void httpTrsx_job_reset(TRSX *trsx)
{
    httpTrsx_setExecMode(trsx, EM_WAIT_NEW_EXEC_MODE);
    trsx->exec.status = IDLE;
    trsx->exec.runInterval_sm0 = 0x00;  //reset
    //
    trsx->do1trsx.sm0 = 0x00;
    trsx->respMsg.sm0 = 0x00;
}
//////////////////////////////
#if defined(SOCKET_DEBUG) && defined(HTTPTRSX_DEBUG)
#include <utility/w5100.h>
//#include <utility/socket.h>

/*("#:Status Port Destination DPort");
("0=avail,14=waiting,17=connected,22=UDP");
("1C=close wait");
 */
void ShowSocketStatus(void)
{
    char str[64];
    char buff[10];
    uint8_t port;

    for (uint8_t i = 0; i < MAX_SOCK_NUM; i++)
    {
    	strcpy(str,"#");
    	cIntToStr(i,buff, sizeof(buff));
    	strcat(str, buff);

    	strcat(str,":0x");
        uint8_t s = W5100.readSnSR(i); //status
        sprintf(buff,"%x",s);
        strcat(str,buff);

        // if(s == 0x1C)
        //{close(i);}

        strcat(str," ");
        port = W5100.readSnPORT(i);//port source
        cIntToStr(port,buff, sizeof(buff));
        strcat(str,buff);

        strcat(str," D:");
        uint8_t destIP[10];
        W5100.readSnDIPR(i, destIP); //
        for (int j=0; j<4; j++)
        {
        	cIntToStr(int(destIP[j]), buff, sizeof(buff));
            strcat(str,buff);

            if (j<3) strcat(str,".");
        }

        strcat(str," (");
        port = W5100.readSnDPORT(i); //port on destination
        cIntToStr(port, buff, sizeof(buff));
		strcat(str,buff);
		strcat(str,") ");
		//
		httpTrsxDebug.UART_println(str, STR_RAM);
    }
}

#endif

