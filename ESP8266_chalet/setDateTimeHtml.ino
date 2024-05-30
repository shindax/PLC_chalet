#include "system.h"

String getDayOption( String title, String index )
{
  String page ="<option value='" + index + "' ";
    if( dayOfWeek == index )
      page += "selected";
      page +=">" + title + "</option>";
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
  
  page +="<input id='set' type='submit' value='Выполнить'>";
  page +="</form>";

  page += "<button onclick='location.href=\"/\"' type='button'>Назад</button>";
   page +="</body></html>";
  return page;
}