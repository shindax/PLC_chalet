#include <ESP8266WebServer.h>        // include ESP8266webserver lib
#include "buildpage.h"          // webpage HTML code

ESP8266WebServer server;        // associate server with library
char ssidAP[] = "ESP8266";        // WLAN SSID and password
char passwordAP[] = "12345678";
IPAddress local_ip(192,168,1,1);      // pre-defined IP address values
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

int LEDGpin = LED_BUILTIN;
String datetime = "";
String dayOfWeek = "";

unsigned char usartBuf[64];

const unsigned char USART_OUTPUTS_MODE_REQUEST = 0x11;
const unsigned char USART_OUTPUTS_MODE_RESPONSE = 0x22;
const unsigned char USART_OUTPUTS_MODE_SET = 0x33;
const unsigned char USART_TIME_SETTING = 0x55;

void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);         // Wi-Fi AP mode
  delay(1000);            // setup AP mode
  WiFi.softAPConfig(local_ip, gateway, subnet); //  predefined IP address
  WiFi.softAP(ssidAP, passwordAP);      // initialise Wi-Fi
  server.begin();           // initialise server
  server.on("/", base);         // load default webpage
  server.on("/setDatetime", setDateTime);
  server.on("/setOutputs", setOutputsMode);       // map URLs to functions:
  pinMode(LEDGpin, OUTPUT);       // define LED pins as output
}

void base()
{
  server.send(200, "text.html", root());
}

void setDateTime()
{
  int argCount = server.args();
  if( argCount ){
    datetime = server.arg("datetime");
    dayOfWeek = server.arg("day");
    char Buf[17];
    datetime.toCharArray(Buf, 17);
    usartBuf[0] = USART_TIME_SETTING;
    usartBuf[1] = getDatetimeValue( Buf, 2 );
    usartBuf[2] = getDatetimeValue( Buf, 5 );
    usartBuf[3] = getDatetimeValue( Buf, 8 );
    usartBuf[4] = getDatetimeValue( Buf, 11 );
    usartBuf[5] = getDatetimeValue( Buf, 14 );
    usartBuf[6] = dayOfWeek.toInt();
    usartBuf[7] = 0xFF;
    Serial.write( usartBuf, 8 );
  }
  server.send(200, "text/html", setDateTimeHtml());
}

void setOutputsMode()
{
  int argCount = server.args();
  int ch1 = server.arg("ch1").toInt();
  int ch2 = server.arg("ch2").toInt();
  int ch3 = server.arg("ch3").toInt();
  int ch4 = server.arg("ch4").toInt();

  usartBuf[0] = USART_OUTPUTS_MODE_REQUEST;
  Serial.write( usartBuf, 8 );
  Serial.flush();
  
  Serial.setTimeout(500);
  //while( Serial.available() < 8 );
  //Serial.read( usartBuf, 8 );
  Serial.readBytes( usartBuf, 8 );

  if( argCount ){
      usartBuf[0] = USART_OUTPUTS_MODE_SET;
      usartBuf[1] = ch1;
      usartBuf[2] = ch2;
      usartBuf[3] = ch3;
      usartBuf[4] = ch4;
    
      Serial.write( usartBuf, 8 );
      Serial.flush();

      delay(100);
    }

  server.send(200, "text/html", setOutputsHtml());
}

unsigned char getDatetimeValue( char * Buf, int offset )
{
  unsigned char ch;
  char tempBuf[3] = {0,0,0};
  strncpy( tempBuf, Buf + offset, 2);
  ch = atoi( tempBuf );
  return convertToHex( ch );
}

unsigned char convertToHex( unsigned char ch )
{
  return ( ch / 10 * 16 ) + ( ch % 10 );
}

void loop()
{
  server.handleClient();        // manage HTTP requests
}
