/* httpTrsx2
  Arduino 1.8.9
  Sep, 19 - 2019
  ]<äf.

int8_t httpTrsx_job(TRSX *trsx, int8_t typeData, void *txmsg, uint16_t txmsgNumMax, char *rxmsg, size_t rxmsgSize, uint16_t *rxCounterChar)

Parameters:
*Param 1: TRSX * trsx: Pointer to the current instance for the current transaction
*Param 2: Type of data to send: TX_CHARARRAY (char []) or TX_JSONSTRUCT (JSON.name/JSON.strval)
*Param 3: Tx-payload: to send to the sever
*Param 4: Length of the Tx-payload: use strlen() for the null-terminated message to send
*Param 5: Rx-Buffer: stream received from the served has be placed in this char array[]
*Param 6: Size of Rx-Buffer: Maximum size available,  use sizeof()

httpTrsx_job can return:
* 0: Transaction is being processed
* 1: Transaction has been completed
* -1: Transaction has been completed with ERRORS: failed a Conexion, Server dropped, etc etc
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
  //
  httpTrsx_setExecMode(&trsx, EM_RUN_REPETITIVE);
  httpTrsx_setExecIntervalRepetitive_ms(&trsx, intervalRepetitive_ms);

  /* Programming in "Arduino-style" */
  digitalWrite(LED7, LOW);/* Make output pin zero for D13=internal Led*/
  pinMode(LED7, OUTPUT);
}
void eth_job(void);
int8_t ethTrsxTxRx_job(void);

void loop(void)
{
  /* TODO: User space */
  /* eth_job(): Is a non-blocking service*/
  eth_job();
}
//Data to send
int8_t ch0_val;

#define CH1_NUM_ELEMENTOS 2
double ch1_val[CH1_NUM_ELEMENTOS];

void eth_job(void)
{
  if (ethTrsxTxRx_job() == 1) //Finish Tx job?
  {
    ch0_val++;
    Serial.println(F("\nExecution mode: EM_RUN_REPETITIVE"));
    Serial.println(F("Make a loopback: Client update and send data, then server make an echo of the last current updated"));
    Serial.print(F("Waiting for inteval of ")); Serial.print(intervalRepetitive_ms); Serial.println(" ms....\n");
  }
}

int8_t ethTrsxTxRx_job(void)
{
  int8_t cod_ret_trsx = 0;
  int8_t httpTrsx_rpta;

#define NUM_JSON_STRUCTS_TX 2
  JSON jsontx[NUM_JSON_STRUCTS_TX];//This struct is for transmition data, Num of structs = 2
  //
  char stream[400];
  JSON json;    //This struct is for decoding the received stream
  uint16_t rxCounterChar;
  int8_t jsonDecode_rpta = 0;

  // Load data to json struct[0]
  jsontx[0].name = "ch0";
  char ch0_strval[20];
  cIntToStr(ch0_val, ch0_strval, sizeof(ch0_strval));
  jsontx[0].strval = ch0_strval;
  //
  // Load data to json struct[1]
  jsontx[1].name = "ch1";
  ch1_val[0] = random(0, 5);
  ch1_val[1] = random(0, 12);
  char ch1_strval[50];
  json_cNumericArrToStr(ch1_val, CNUM_FLOAT, CH1_NUM_ELEMENTOS, ch1_strval);
  jsontx[1].strval = ch1_strval;

  /* The last built json will bed taked and send only once when the INTERVAL expires*/
  rxCounterChar = 0x0000;
  httpTrsx_rpta = httpTrsx_job(&trsx, TX_JSONSTRUCT, &jsontx, NUM_JSON_STRUCTS_TX, stream, sizeof(stream), &rxCounterChar);//In this example, Only write data to the server
  if (rxCounterChar > 0)
  {
    if (rxCounterChar >= sizeof(stream))/* Here User can detect if buffer need to grow */
    {
      Serial.println(F("Increase size of stream[]"));/* Incomplete decoding...data loss to Json decoding */
    }
    else
    {
      stream[rxCounterChar] = '\0';/* Make the stream buffer as string null-terminated */

      Serial.print(F("rx stream is:")); Serial.println(stream);
      do
      {
        json.name = (char*) NULL; //init a NULL, is after decode found a valid json, then json.name is not NULL
        /*json_decode return 1 if decoding has been complete, else 0 if is processing*/
        jsonDecode_rpta = json_decode(stream, sizeof(stream), &json); //search in the current available stream[], if found stop and parse, then, continue searching until
        if (json.name != NULL)                    //jsonDecode_rpta == 1.. see do{}while (jsonDecode_rpta == 0)
        {
          /* if json.name is diferente to NULL, means found a valid JSON data as a pair */
          if (strcmp(json.name, "ch0") == 0)
          {
            int ch0_val_rx = cStrToInt(json.strval);
            Serial.print(F("ch0, last updated value = ")); Serial.println(ch0_val_rx);
          }
          else if (strcmp(json.name, "ch1") == 0)
          {
            double ch1_val_rx[CH1_NUM_ELEMENTOS];
            json_cDecStrArr_To_DoubleArr(json.strval, ch1_val_rx, CH1_NUM_ELEMENTOS);
            Serial.print(F("ch1[0], last random value = ")); Serial.println(ch1_val_rx[0]);
            Serial.print(F("ch1[1], last random value = ")); Serial.println(ch1_val_rx[1]);
          }
        }
      }
      while (jsonDecode_rpta == 0);
    }
  }
  if ((httpTrsx_rpta == 1) || (httpTrsx_rpta == -1))
  {
    cod_ret_trsx = 1;//End transaction
  }
  return cod_ret_trsx;
}
