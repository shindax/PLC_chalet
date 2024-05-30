#include "system.h"

String getTimeInput( String hour, String minute, String name )
{
  return String("<input name='" + name + "' type='time' value='" + hour + ":" + minute + "' " + "/>");
}

String leftPad( unsigned char ch )
{
  String str = String( ch, HEX );
  if( str.length() == 1 )
    str = "0" + str;
  return str;
}

String makeChannelSelectOption( unsigned char ch )
{
  unsigned char i;
  String str = "";
  String strArr[5] = {"ВЫКЛ", "1", "2", "3", "4"};
  if( ch == 0xFF )
    ch = 0;
  for( i = 0 ; i < 5; i ++ ){
      str += "<option value='";
      if( i == 0 )
          str += 0xFF;
            else
              str += i;
      str += "' ";
      if( ch == i )
        str += "selected>";
        else
          str += ">";

      str += strArr[i];
      str += "</option>";
    }
  return str;
}

String makeChannelSelect( unsigned char ch, unsigned char addr )
{
  String str = "<select name='channel";
  str += addr;
  str += "'>";
  str += makeChannelSelectOption( ch );
  str += "</select>";
  return str;
}

String getSettingsTr( int i )
{
  unsigned char addr = usartBuf[ i + 7 ];
  
  String str = "<tr>";

  str += "<td>";  
  str += String( addr / 8 + 1 );
  str += "</td>";

  str += "<td>";  
  str += makeChannelSelect( usartBuf[ i ], addr );
  str += "</td>";
    
  str += "<td>";
  str += getTimeInput( leftPad( usartBuf[ i + 2 ] ), leftPad( usartBuf[ i + 3 ] ), String("timeFrom") + String( addr ));
  str += "</td>";

  str += "<td>";
  str += getTimeInput( leftPad( usartBuf[ i + 4 ] ), leftPad( usartBuf[ i + 5 ] ), String("timeTo") + String( addr ) );
  str += "</td>";

  str += "</tr>";

  return str;
}

String setAlarmSettingsBankHtml( unsigned char bank )
{
  String page = getHtmlHeader();
  page += "<body>";
  page += "<h1>Уставки выходов</h1>"; 
  page += "<h2>Банк ";
  page += bank;
  page += "</h2>";  

  page +="<form method='get' action='/setAlarmSettingsBank";
  page += bank;
  page += "'>";
  page +="<input type='hidden' name='bank' value='";
  page += bank;
  page += "'>";

  page +="<table class='time-settings'>";
  
    for( unsigned int i = 0; i < 64; i +=8  )
      page += getSettingsTr( i );
  
  page +="</table>";
  page +="<input id='set' type='submit' value='Выполнить'>";
  page += "</form>";

  page += "<button onclick='location.href=\"/getAlarmSettings\"' type='button'>Назад</button>";
  page += "</body></html>";
  return page;
}