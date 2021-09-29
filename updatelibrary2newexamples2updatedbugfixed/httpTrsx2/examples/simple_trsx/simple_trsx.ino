/* httpTrsx2
  Arduino 1.8.9
  Sep, 19 - 2019
  ]<äf.
 */
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <httpTrsx2.h>

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

/*1 Instance */
TRSX trsx;

EthernetClient client;  //1 arduino client instance

unsigned long intervalRepetitive_ms = 2000;
void eth_job(void);
int8_t ethTrsxTxRx_job(void);

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
  httpTrsx_setClient(&trsx, (Client*) &client);        //Asign "Arduino-Model client"
  httpTrsx_setupServerByIP(&trsx, IPaddr_server, 80);
  httpTrsx_setURI(&trsx, "/jsondecodeTxRx.php");
  httpTrsx_setHost(&trsx, "192.168.1.48");             //virtual host in the server
  httpTrsx_setHdrLine(&trsx, "api_key_write: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
  httpTrsx_setExecMode(&trsx, EM_RUN_ONCE);            //set Trsx Execution Mode, if EM_RUN_ONCE, then user can be reload after finish the current transaction

  /* Programming in "Arduino-style" */
  digitalWrite(LED7, LOW);/* Make output pin zero for D13=internal Led*/
  pinMode(LED7, OUTPUT);
}
void loop(void)
{
  /* TODO: User space */

  /* eth_job(): Is a non-blocking service*/
  eth_job();
}

void eth_job(void)
{
  static unsigned long millis_last;
  static int8_t timming;

  if (!timming)
  {
    if (ethTrsxTxRx_job() == 1) //Finish Tx job?
    {
      Serial.println(F("A Tx/RX trsx end\n"));
      Serial.println(F("\nExecution mode: EM_RUN_ONCE"));
      Serial.println(F("Make a loopback: Client update and send data, then server make an echo of the last current updated"));
      Serial.print(F("Waiting for inteval of ")); Serial.print(intervalRepetitive_ms); Serial.println(" ms....\n");
      //
      millis_last = millis();
      timming = 1;
    }
  }
  else
  {
    if (millis() - millis_last >= intervalRepetitive_ms) //Time(milliseconds) interval between Tx/Rx job
    {
      httpTrsx_setExecMode(&trsx, EM_RUN_ONCE);//for next execution
      timming  = 0x00;
    }
  }

}

/*Param 1: TRSX * trsx: Pointer to the current instance for the current transaction
*Param 2: Type of data to send: TX_CHARARRAY (char []) or TX_JSONSTRUCT (JSON.name/JSON.strval)
*Param 3: Tx-payload: to send to the sever
*Param 4: Length of the Tx-payload: use strlen() for the null-terminated message to send
*Param 5: Rx-Buffer: stream received from the served has be placed in this char array[]
*Param 6: Size of Rx-Buffer: Maximum size available,  use sizeof()
*/
/*httpTrsx_job can return:
* 0: Transaction is being processed
* 1: Transaction has been completed
* -1: Transaction has been completed with ERRORS: failed a Conexion, Server dropped, etc etc
*/
int8_t ethTrsxTxRx_job(void)
{
  int8_t cod_ret_trsx = 0;
  int8_t httpTrsx_rpta;
  static int8_t sm0;
  //
  char jsonCharArray[400];
  char stream[400];
  JSON json;
  uint16_t rxCounterChar;
  int8_t jsonDecode_rpta = 0;

  int8_t ledstatus;

  if (sm0 == 0)
  {
    //ch0 is a long array (aka int32_t), 2 elements
    //ch1 is a float array, 2 elements
    strcpy(jsonCharArray, "{\"ch0\":[-2147483647,2147483646],\"ch1\":[12.45,58.69]}");
    sm0++;
    Serial.print(F("Tx->built JSON:")); Serial.println(jsonCharArray);
  }
  if (sm0 == 1)
  {
    rxCounterChar = 0x0000;
    httpTrsx_rpta = httpTrsx_job(&trsx, TX_CHARARRAY, jsonCharArray, strlen(jsonCharArray), stream, sizeof(stream), &rxCounterChar);//In this example, Only write data to the server

    if (rxCounterChar > 0)
    {
      if (rxCounterChar >= sizeof(stream))/* Here User can detect if buffer need to grow */
      {
        Serial.println(F("Increase size of stream[]"));/* Incomplete decoding...data loss to Json decoding */
      }
      else
      {
        stream[rxCounterChar] = '\0';/* Make the stream buffer as string null-terminated */
        //
        Serial.print(F("Rx <-stream is:")); Serial.println(stream);

        do //begin safe decoding
        {
          json.name = (char*) NULL; //init a NULL, is after decode found a valid json, then json.name is not NULL

          /*json_decode return 1 if decoding has been complete, else 0 if is processing*/
          jsonDecode_rpta = json_decode(stream, sizeof(stream), &json); //search in the current available stream[], if found stop and parse, then, continue searching until
          if (json.name != NULL)                    //jsonDecode_rpta == 1.. see do{}while (jsonDecode_rpta == 0)
          {
            /* if json.name is diferente to NULL, means found a valid JSON data as a pair */
            //Uncomment if you want see the json data pair obtained
            //Serial.print(F("Rx<-a JSON data decoded: "));
            //Serial.print(String(json.name)); Serial.print(":"); Serial.println(String(json.strval));

            if (strcmp(json.name, "ch0") == 0)
            {
              #define CH0_NUM_ELEMENTOS 2
              long ch0_val_rx[CH0_NUM_ELEMENTOS];//2 elements
              json_cLongStrArr_To_LongArr(json.strval, ch0_val_rx, CH0_NUM_ELEMENTOS);
              Serial.print(F("ch0[0] long value = ")); Serial.println(ch0_val_rx[0]);
              Serial.print(F("ch0[1] long value = ")); Serial.println(ch0_val_rx[1]);
            }
            else if (strcmp(json.name, "ch1") == 0)
            {
              #define CH1_NUM_ELEMENTOS 2
              double ch1_val_rx[CH0_NUM_ELEMENTOS];//2 elements
              json_cDecStrArr_To_DoubleArr(json.strval, ch1_val_rx, CH1_NUM_ELEMENTOS);
              Serial.print(F("ch1[0] float value = ")); Serial.println(ch1_val_rx[0]);
              Serial.print(F("ch1[1] float value = ")); Serial.println(ch1_val_rx[1]);
            }

          }
        }
        while (jsonDecode_rpta == 0);
      }
    }
    if ((httpTrsx_rpta == 1) || (httpTrsx_rpta == -1))
    {
      sm0 = 0;
      cod_ret_trsx = 1;//End transaction
    }
  }
  return cod_ret_trsx;
}

