
extern String datetime;
extern String dayOfWeek;
extern unsigned char usartBuf[];
extern ESP8266WebServer server;

String getDayOption( String title, String index )
{
  String page ="<option value='" + index + "' ";
    if( dayOfWeek == index )
      page += "selected";
      page +=">" + title + "</option>";
  return page;
}

String getHtmlHeader()
{
  String page = "<!DOCTYPE html><html><head>";
  page += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>";
  page +="<title>Local network</title>";
  page +="<style>";
  page +="body {margin-top:50px; font-family:Arial; text-align:center;}";
  page +="table {width:100%;}";  
  page +=".btn {width:800px;margin:20px auto; padding:30px; background: #48D1CC;";
  page +="font-size:60px; color:black; text-decoration:none;vertical-align:middle;}";
  page +="input, button{padding: 20px; font-size:80px; margin-top:50px; margin-bottom:5px; width:700px;text-align:center;}";
  page +="select{font-size:80px; margin: 20px 0; width:700px;text-align:center;}";  
  page +="h1, a {font-size:80px; margin:50px auto; text-align:center;}";
  page +="a{ width:700px;height:200px;display: flex;justify-content: center;align-items: center;}";
  page +="button {background: #F0FFF0}";
  page +="span {font-size:80px; margin-right: 20px}";
  page +="tr.gray {vertical-align:middle;}";
  page +="tr.gray {background:#EEE;}";
  page +="input[type='submit']:{background: #7FFF00}";
  page +="input[type='checkbox']:{transform:scale(2);-webkit-transform: scale(2);}";  

  page +="</style></head>";
  return page;
}

String setDateTimeHtml()
{
  String page = getHtmlHeader();
  page +="<body>";
  page +="<h1>Установка времени и даты</h1>";

  page +="<form method='get' action='/setDatetime'>";
  page +="<input type='datetime-local' name='datetime' value='" + datetime + "'>";
  
  page +="<select name='day'>";
  page += getDayOption( "Понедельник", "1" );
  page += getDayOption( "Вторник", "2" );
  page += getDayOption( "Среда", "3" );
  page += getDayOption( "Четверг", "4" );
  page += getDayOption( "Пятница", "5" );
  page += getDayOption( "Суббота", "6" );
  page += getDayOption( "Воскресение", "7" );
  page +="</select>";
  
  page +="<input type='submit' value='Установить'>";
  page +="</form>";

  page += "<button onclick='location.href=\"/\"' type='button'>Назад</button>";
  return page;
}


String getChannelOption( int index, int value )
{
  String modeArr[3] = {"OFF", "ON", "AUTO"};
  String option = "<option value='";
  option += index;
  option += "' ";
  
  if( index == value )
    option += "selected";
  
  option += ">";
  option += modeArr[index];
  option += "</option>";
  return option;
}

String getChannelTd( int index )
{
  String td = "<td><span>CH ";
  td += index;
  td += "</span><select name='ch";
  td += index;
  td += "'>";
  
  td += getChannelOption( 2, usartBuf[ index ] );
  td += getChannelOption( 1, usartBuf[ index ] );
  td += getChannelOption( 0, usartBuf[ index ] );

  td += "</select></td>";
  return td;
}


String setOutputsHtml()
{
  String page = getHtmlHeader();
  page += "<body>";
  page += "<h1>Режим управление выходами</h1>";
  
  page += "<form action='/setOutputs' method='get'>";
  
  page +="<table>";
  
  page +="<tr class='gray'>";
  page += getChannelTd( 1 );
  page +="</tr>";

  page +="<tr>";
  page += getChannelTd( 2 );
  page +="</tr>";

  page +="<tr class='gray'>";
  page += getChannelTd( 3 );
  page +="</tr>";

  page +="<tr>";
  page += getChannelTd( 4 );
  page +="</tr>";

  page +="</table>";
  page +="<input type='submit' value='Установить'>";
  page += "</form>";

  page += "<button onclick='location.href=\"/\"' type='button'>Назад</button>";
  return page;
}

String root()
{
  String page = getHtmlHeader();
  page +="<body>";
  page +="<h1>Настройки системы</h1>";
  page +="<table>";
  page +="<tr><td><div><a href='/setDatetime' class='btn'>Установка времени и даты</a></div></td></tr>";
  page +="<tr><td><div><a href='/setDatetime' class='btn'>Установки управления</a></div></td></tr>";
  page +="<tr><td><div><a href='/setOutputs' class='btn'>Режим управление выходами</a></div></td></tr>";  
  page +="</table>";
  page +="</body></html>";
  return page;
}

