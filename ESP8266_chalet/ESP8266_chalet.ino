#include <ESP8266WebServer.h>
#include "system.h"

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

const unsigned char USART_OUTPUTS_MODE_REQUEST = 0x44;
const unsigned char USART_OUTPUTS_MODE_SET = 0x33;
const unsigned char USART_TIME_SETTING = 0x55;
const unsigned char USART_OUTPUTS_ALARM_SETTINGS_REQUEST = 0x66;
const unsigned char USART_OUTPUT_ALARM_SETTINGS_SET = 0x77;
const unsigned char DISPLAY_ON                      = 0x88;
const unsigned char DISPLAY_OFF                     = 0x99;
const unsigned char USART_GET_OUTPUTS_STATE					= 0xAA;

void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);         // Wi-Fi AP mode
  delay(1000);            // setup AP mode
  WiFi.softAPConfig(local_ip, gateway, subnet); //  predefined IP address
  WiFi.softAP(ssidAP, passwordAP);      // initialise Wi-Fi
  server.begin();
  server.on("/", base);
  server.on("/setDatetime", setDateTime);
  server.on("/setOutputs", setOutputsMode);
  server.on("/getAlarmSettings", getAlarmSettings);
    
  server.on("/setAlarmSettingsBank1", setAlarmSettingsBank1);
  server.on("/setAlarmSettingsBank2", setAlarmSettingsBank2);
  server.on("/setAlarmSettingsBank3", setAlarmSettingsBank3);
  server.on("/setAlarmSettingsBank4", setAlarmSettingsBank4);
  
  server.on("/display", display);
  server.on("/displayOn", displayOn);
  server.on("/displayOff", displayOff);
  server.on("/outputStates", outputStates);
  pinMode(LEDGpin, OUTPUT);       // define LED pins as output
  pinMode(2, OUTPUT);       // define LED pins as output
}

void base()
{
  server.send(200, "text.html", rootHtml());
}

void display()
{
  server.send(200, "text.html", displayHtml());
}

void displayOn()
{
  unsigned char tempUsartBuf[8];  
  tempUsartBuf[0] = DISPLAY_ON;
  Serial.write( tempUsartBuf, 8 );
  server.send(200, "text.html", displayHtml());
}

void displayOff()
{
  unsigned char tempUsartBuf[8];  
  tempUsartBuf[0] = DISPLAY_OFF;
  Serial.write( tempUsartBuf, 8 );
  server.send(200, "text.html", displayHtml());
}

void outputStates()
{
  unsigned char tempUsartBuf[4];
  tempUsartBuf[0] = USART_GET_OUTPUTS_STATE;
  Serial.write( tempUsartBuf, 8 );
  Serial.flush();
  Serial.setTimeout(300);
  Serial.readBytes( usartBuf, 8 );
  server.send(200, "text.html", outputStatesHtml());
}

void getAlarmSettings()
{
  server.send(200, "text.html", getAlarmSettingsHtml());
}


void getUsartAlarmSettings( unsigned char bank )
{
  unsigned char addr = ( bank - 1 ) * 64;
  unsigned char tempUsartBuf[8];
  Serial.flush();
  for( unsigned char i = 0; i < 8; i ++ ){
    tempUsartBuf[0] = USART_OUTPUTS_ALARM_SETTINGS_REQUEST;
    tempUsartBuf[1] = addr + i * 8;
    Serial.write( tempUsartBuf, 8 );
    delay(50);
    Serial.setTimeout(500);
    Serial.readBytes( usartBuf + i * 8, 8 );
  }
}

void setAlarmSettings()
{
  int argCount = server.args();
  if( argCount ){
    String bank = server.arg("bank");
    unsigned int base = ( bank.toInt() - 1 ) * 64;
    unsigned char tempUsartBuf[8];
    Serial.flush();
    for( int i = 0; i < 8; i ++ ){

      String suffix = String( base + i * 8 );
      unsigned int channel = server.arg( String("channel") + suffix ).toInt();
      tempUsartBuf[0] = USART_OUTPUT_ALARM_SETTINGS_SET;
      tempUsartBuf[1] = suffix.toInt();
      tempUsartBuf[2] = channel;
      tempUsartBuf[3] = 0x7F;
      String timeFrom = server.arg( String("timeFrom") + suffix );
      String timeTo = server.arg( String("timeTo") + suffix );
      tempUsartBuf[4] = dec2hex( timeFrom.substring(0,2).toInt());  // from hour
      tempUsartBuf[5] = dec2hex( timeFrom.substring(3).toInt());    // from minute
      tempUsartBuf[6] = dec2hex( timeTo.substring(0,2).toInt());    // to hour
      tempUsartBuf[7] = dec2hex( timeTo.substring(3).toInt());      // to minute
        
      Serial.write( tempUsartBuf, 8 );
       delay(300);
    }// for( int i = 0; i < 8; i ++ ){

  }// if( argCount ){
}//void setAlarmSettings()

void setAlarmSettingsBank1()
{
  setAlarmSettings();
  getUsartAlarmSettings(1);
  server.send(200, "text.html", setAlarmSettingsBankHtml(1));
}

void setAlarmSettingsBank2()
{
  setAlarmSettings();
  getUsartAlarmSettings(2);
  server.send(200, "text.html", setAlarmSettingsBankHtml(2));
}

void setAlarmSettingsBank3()
{
  setAlarmSettings();
  getUsartAlarmSettings(3);
  server.send(200, "text.html", setAlarmSettingsBankHtml(3));
}

void setAlarmSettingsBank4()
{
  setAlarmSettings();
  getUsartAlarmSettings(4);
  server.send(200, "text.html", setAlarmSettingsBankHtml(4));
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
  usartBuf[0] = USART_OUTPUTS_MODE_REQUEST;
  Serial.write( usartBuf, 8 );
  Serial.flush();
  
  Serial.setTimeout(500);
  Serial.readBytes( usartBuf, 8 );

  if( argCount ){
    int ch1 = server.arg("ch1").toInt();
    int ch2 = server.arg("ch2").toInt();
    int ch3 = server.arg("ch3").toInt();
    int ch4 = server.arg("ch4").toInt();

    usartBuf[0] = USART_OUTPUTS_MODE_SET;
    usartBuf[1] = ch1;
    usartBuf[2] = ch2;
    usartBuf[3] = ch3;
    usartBuf[4] = ch4;
    Serial.write( usartBuf, 8 );
    Serial.flush();
    delay(200);
    }

  server.send(200, "text/html", setOutputsHtml());
}

unsigned char getDatetimeValue( char * Buf, int offset )
{
  unsigned char ch;
  char tempBuf[3] = {0,0,0};
  strncpy( tempBuf, Buf + offset, 2);
  ch = atoi( tempBuf );
  return dec2hex( ch );
}

unsigned char dec2hex( unsigned char ch )
{
  return ch / 10 * 16 + ( ch % 10 );
}

void loop()
{
  server.handleClient();        // manage HTTP requests
}
