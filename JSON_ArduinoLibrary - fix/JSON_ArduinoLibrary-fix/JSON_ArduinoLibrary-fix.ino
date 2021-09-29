/* httpTrsx2
   Arduino 1.8.9

   ArduinoJson - arduinojson.org v6.0
   Copyright Benoit Blanchon 2014-2019
   MIT Licens
	Sep, 19 - 2019
   ]<äf.
*/
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <httpTrsx2.h>
#include <ArduinoJson.h>

void spi_deselect_devices(void)//only for QUANTICO BOARD
{
#define WIZNET_CS 10
#define SDCARD_CS 4
#define LTC6804_CS 5
	pinMode(WIZNET_CS, OUTPUT);
	pinMode(SDCARD_CS, OUTPUT);
	pinMode(LTC6804_CS, OUTPUT);
	digitalWrite(WIZNET_CS, HIGH);
	digitalWrite(SDCARD_CS, HIGH);
	digitalWrite(LTC6804_CS, HIGH);
}

#define RELAY1 29//2
#define RELAY2 30//3
#define RELAY3 31//4
#define RELAY4 32//5
#define RELAY5 33//6
//
#define LED1 34//7
#define LED2 35//8
#define LED3 36//9
#define LED4 37//10
#define LED5 38//11
#define LED6 39//12
//
#define LED7 13//internal LED

/*NIC local setup*/
struct _myNIC
{
	uint8_t MAC[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	uint8_t IPfallback[4] = { 192, 168, 1, 60 };  //fallback IP if DHCP can't lease
} myNIC;

/*Connect to server - setting*/
//char server[] = "api.quanticoservices.net";
IPAddress IPaddr_server(192, 168, 1, 48);

/* Instance n TRSX to use, trsx[0] for write to server and trsx[1] for read from server*/
#define TRSX_NUMMAX 2
TRSX trsx[TRSX_NUMMAX];

EthernetClient client;  //Instance

void setup(void)
{
	Serial.begin(38400);
	Serial.println(F("Welcome..."));

	spi_deselect_devices(); //only for QUANTICO BOARD

	/* local network setting */
	NIC_begin(myNIC.MAC, myNIC.IPfallback); //by default request IP by DHCP, if fail fallback-IP is taked

	/* optional: print my IP*/
	char buff[30];
	NIC_getMyIP(buff, sizeof(buff));
	Serial.println("My IP: " + String(buff));

	Serial.println(F("Give 2 seconds to initialize the NIC...\n"));
  	delay(2000);

	/* Set trsx[0] for Tx to server */
	httpTrsx_setClient(&trsx[0], (Client*) &client);        //Asign "Arduino-Model client"
	httpTrsx_setupServerByIP(&trsx[0], IPaddr_server, 80);
	httpTrsx_setURI(&trsx[0], "/jsondecodeTx.php");
	httpTrsx_setHost(&trsx[0], "192.168.1.48");             //virtual host in the server
	httpTrsx_setHdrLine(&trsx[0], "api_key_write: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
	httpTrsx_setExecMode(&trsx[0], EM_RUN_ONCE);            //set Trsx Execution Mode, if EM_RUN_ONCE, then user can be reload after finish the current transaction

	/* Set trsx[1] for Rx from server */
	httpTrsx_setClient(&trsx[1], (Client*) &client);
	httpTrsx_setupServerByIP(&trsx[1], IPaddr_server, 80);
	httpTrsx_setURI(&trsx[1], "/jsondecodeRx.php");
	httpTrsx_setHost(&trsx[1], "192.168.1.48");
	httpTrsx_setHdrLine(&trsx[1], "api_key_read: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
	httpTrsx_setExecMode(&trsx[1], EM_RUN_ONCE);

	/* Programming in "Arduino-style" */
	/* Make output pin zero for Leds vector */
	digitalWrite(RELAY1, LOW);
	pinMode(RELAY1, OUTPUT);
	digitalWrite(RELAY2, LOW);
	pinMode(RELAY2, OUTPUT);
	digitalWrite(RELAY3, LOW);
	pinMode(RELAY3, OUTPUT);
	digitalWrite(RELAY4, LOW);
	pinMode(RELAY4, OUTPUT);
	digitalWrite(RELAY5, LOW);
	pinMode(RELAY5, OUTPUT);

	/* Make output pin zero for Relay vector */
	digitalWrite(LED1, LOW);
	pinMode(LED1, OUTPUT);
	digitalWrite(LED2, LOW);
	pinMode(LED2, OUTPUT);
	digitalWrite(LED3, LOW);
	pinMode(LED3, OUTPUT);
	digitalWrite(LED4, LOW);
	pinMode(LED4, OUTPUT);
	digitalWrite(LED5, LOW);
	pinMode(LED5, OUTPUT);
	digitalWrite(LED6, LOW);
	pinMode(LED6, OUTPUT);
	/* Make output pin zero for D13=internal Led*/
	digitalWrite(LED7, LOW);
	pinMode(LED7, OUTPUT);
}
void eth_job(void);
int8_t ethTrsxTx_job(void);
int8_t ethTrsxRx_job(void);

void loop(void)
{
	/* TODO: User space */

	/* eth_job(): Is a non-blocking service*/
	eth_job();
}

void eth_job(void)
{
	static unsigned long millis_last;
	static int8_t sm0;
	static int8_t timming;

	if (!timming)
	{
		if (sm0 == 0)
		{
			if (ethTrsxTx_job() == 1) //Finish Tx job?
			{
				Serial.println(F("A Tx-trsx end\n"));
				//
				millis_last = millis();
				timming = 1;
				sm0++;
			}
		}
		else if (sm0 == 1)
		{
			if (ethTrsxRx_job() == 1)//Finish Rx job?
			{
				Serial.println(F("A Rx-trsx end\n"));
				//
				millis_last = millis();
				timming = 1;
				sm0 = 0x00;
			}
		}
	}
	else
	{
		if (millis() - millis_last >= 1000) //Time(milliseconds) interval between Tx/Rx job
			timming  = 0x00;
	}

}

/*data to write to the server:*/
#define NUM_IC 6
#define LTC_NUM_CELLS 6
float volts[NUM_IC] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
uint8_t uv_flag[NUM_IC][LTC_NUM_CELLS] =
{
	{ 1, 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 1, 0, 1 },
};

int8_t ethTrsxTx_job(void)
{
	int8_t cod_ret_trsx = 0;

	char jsonCharArray[150];
	DynamicJsonDocument  doc(400); //v6.0.. Is very insane this heap memory...Can't be 150 because uv_flag cannot fit!!!! Then I reserver 400 bytes

	char buff[50];
	int8_t httpTrsx_rpta;
	static int8_t sm0;

	if (sm0 == 0)
	{
		//Use Community Json Library
		JsonArray data;
		data = doc.createNestedArray("volts");
		for (int i = 0; i < NUM_IC; i++)
			data.add(volts[i]);

		//I don't know if JSON library can format to this nested array: "uv_flag":[[1,0,1,0,1,0],[0,1,0,1,0,1],[1,0,1,0,1,0],[0,1,0,1,0,1],[1,0,1,0,1,0],[0,1,0,1,0,1]]
		//By now, I build as a one plain array[]. eg "uv_flag":[1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1]
		JsonArray data1 = doc.createNestedArray("uv_flags");
		for (int i = 0; i < NUM_IC; i++)
		{
			for (int j = 0; j < LTC_NUM_CELLS; j++)
			{
				data1.add(uv_flag[i][j]);
			}
		}
		//
		String output;
		serializeJson(doc, output);
		output.toCharArray(jsonCharArray, sizeof(jsonCharArray));
		//
		sm0++;
		Serial.print(F("Tx->built JSON:")); Serial.println(jsonCharArray);
	}
	if (sm0 == 1)
	{
		/*Param 1: TRSX * trsx: Pointer to the current instance for the current transaction
		  Param 2: Type of data to send: TX_CHARARRAY (char []) or TX_JSONSTRUCT (JSON.name/JSON.strval)
		  Param 3: Tx-payload: to send to the sever
		  Param 4: Length of the Tx-payload: use strlen() for the null-terminated message to send
		  Param 5: Rx-Buffer: stream received from the served has be placed in this char array[]
		  Param 6: Size of Rx-Buffer: Maximum size available,  use sizeof()
		*/
		httpTrsx_rpta = httpTrsx_job(&trsx[0], TX_CHARARRAY, jsonCharArray, strlen(jsonCharArray), (char*)NULL, 0, NULL);//In this example, Only write data to the server
		/*httpTrsx_job can return:
		   0: Transaction is being processed
		   1: Transaction has been completed
		   -1: Transaction has been completed with ERRORS: failed a Conexion, Server dropped, etc etc
		*/
		if ((httpTrsx_rpta == 1) || (httpTrsx_rpta == -1))
		{
			sm0 = 0x00;
			httpTrsx_setExecMode(&trsx[0], EM_RUN_ONCE);//for next execution
			cod_ret_trsx = 1;
		}
	}
	return cod_ret_trsx;
}

//StaticJsonDocument<250> doc;//in stack
int8_t ethTrsxRx_job(void)
{
	int8_t cod_ret_trsx = 0;
	//
  #define RX_STREAM_SIZE 200
	char stream[RX_STREAM_SIZE];

	//JSON json;
	DynamicJsonDocument  doc(250);//I don't know doc() need more size than stream[] ???
 	//StaticJsonDocument<250> doc;//in stack
	
	int8_t httpTrsx_rpta;
	int ledstatus;
	
	uint16_t rxCounterChar;
	//
	rxCounterChar = 0x00;	//first counter to zero
	httpTrsx_rpta = httpTrsx_job(&trsx[1], TX_CHARARRAY, (char*)NULL, 0, stream, sizeof(stream), &rxCounterChar);//In this example, Only read data from server
	
	//Server send : {"d13":1,"rele":[1,1,1,1,1],"led":[1,1,1,1,1,1],"volt":[7.75,8.86,9.97,10.18,11.19,12.33],"text":["open","true"]}
	
	if (rxCounterChar > 0)	//Any char was written?
	{
		Serial.println("Number of read characters:" + String(rxCounterChar));
		
		/* Here User can dectect if buffer need to grow */
		if (rxCounterChar >= sizeof(stream))
		{
			//Incomplete decoding...data loss to Json decoding decoding
			Serial.println(F("Increase size of stream[]"));
		}
		else
		{
      stream[RX_STREAM_SIZE-1] = '\0';
      Serial.print("rx stream-sizeof(stream) is:");Serial.println(stream);
			
			/* Make the stream buffer as string null-terminated */
			stream[rxCounterChar] = '\0';
      Serial.print("rx stream\0 is:");Serial.println(stream);
     
			//
      int idx=0;
      while (stream[idx++] != '{')  {;}
      String s = String(&stream[idx-1]);
			//String s = String(stream);
			Serial.print(F("rx String(stream \0) is:"));Serial.println(s);

			deserializeJson(doc, s);
			JsonObject obj = doc.as<JsonObject>();

			//"d13":1
			ledstatus = obj["d13"].as<int>();
			(ledstatus == 1) ? digitalWrite(LED7, HIGH) : digitalWrite(LED7, LOW);
      (ledstatus > 0) ? Serial.println("digitalWrite(LED7, HIGH)"): Serial.println("digitalWrite(LED7, LOW)");

			//"rele":[1,1,1,1,1]
			(obj["rele"][0].as<int>() == 1) ? digitalWrite(RELAY1, HIGH) : digitalWrite(RELAY1, LOW);
			(obj["rele"][1].as<int>() == 1) ? digitalWrite(RELAY2, HIGH) : digitalWrite(RELAY2, LOW);
			(obj["rele"][2].as<int>() == 1) ? digitalWrite(RELAY3, HIGH) : digitalWrite(RELAY3, LOW);
			(obj["rele"][3].as<int>() == 1) ? digitalWrite(RELAY4, HIGH) : digitalWrite(RELAY4, LOW);
			(obj["rele"][4].as<int>() == 1) ? digitalWrite(RELAY5, HIGH) : digitalWrite(RELAY5, LOW);

			//"led":[1,1,1,1,1,1]
			(obj["led"][0].as<int>() == 1) ? digitalWrite(LED1, HIGH) : digitalWrite(LED1, LOW);
			(obj["led"][1].as<int>() == 1) ? digitalWrite(LED2, HIGH) : digitalWrite(LED2, LOW);
			(obj["led"][2].as<int>() == 1) ? digitalWrite(LED3, HIGH) : digitalWrite(LED3, LOW);
			(obj["led"][3].as<int>() == 1) ? digitalWrite(LED4, HIGH) : digitalWrite(LED4, LOW);
			(obj["led"][4].as<int>() == 1) ? digitalWrite(LED5, HIGH) : digitalWrite(LED5, LOW);
			(obj["led"][5].as<int>() == 1) ? digitalWrite(LED6, HIGH) : digitalWrite(LED6, LOW);

			//"volt":[7.75,8.86,9.97,10.18,11.19,12.33]
			/* Update global Array "volts" from the jsondecodeRx.php, the json "volt":[,,,,] */
			for (int i = 0; i < NUM_IC; i++)
			{volts[i] = obj[String("volt")][i].as<double>();}

			/* ArduinoJson library remove \" \" in text values... I think is better but, PC json decoder always return with \" \" */
			//"text":["open","true"]
			const char *str_state = obj["text"][0].as<char*>();//index 0 is for open/close
			if (strcmp(str_state, "open") == 0)
			{
				Serial.println(F("TODO: Anything when is open"));
			}
			else if (strcmp(str_state, "close") == 0)
			{
				Serial.println(F("TODO: Anything when is close"));
			}
			//
			const char *str_boolean = obj["text"][1].as<char*>();//index 1 is for true/false
			if (strcmp(str_boolean, "true") == 0)
			{
				Serial.println(F("TODO: Anything when is true"));
			}
			else if (strcmp(str_boolean, "false") == 0)
			{
				Serial.println(F("TODO: Anything when is false"));
			}
		}

	}
	if ((httpTrsx_rpta == 1) || (httpTrsx_rpta == -1))
	{
		httpTrsx_setExecMode(&trsx[1], EM_RUN_ONCE);//for next execution
		cod_ret_trsx = 1;
	}
	return cod_ret_trsx;
}
