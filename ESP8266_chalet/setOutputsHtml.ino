#include "system.h"

String getChannelOption( int index, int value )
{
  String modeArr[3] = {"ВЫКЛ", "ВКЛ", "АВТО"};
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
  String td = "<td><span>ВЫХОД ";
  td += index;
  td += "</span><select class='channel' name='ch";
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
  page += "<h1>Режим управления выходами</h1>";
  
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
  page +="<input id='set' type='submit' value='Выполнить'>";
  page += "</form>";

  page += "<button onclick='location.href=\"/\"' type='button'>Назад</button>";
  page +="</body></html>";
  return page;
}
