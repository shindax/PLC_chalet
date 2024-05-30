extern String datetime;
extern String dayOfWeek;
extern unsigned char usartBuf[];
extern ESP8266WebServer server;

String getHtmlHeader( void );
String getAlarmSettingsHtml( void );
String outputStatesHtml( void );
String setOutputsHtml( void );
String displayHtml( void );
String setDateTimeHtml( void );
String rootHtml( void );
unsigned char dec2hex( unsigned char ch );