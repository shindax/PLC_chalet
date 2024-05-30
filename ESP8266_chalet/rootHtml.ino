#include "system.h"

String rootHtml()
{
  String page = getHtmlHeader();
  page +="<body>";
  page +="<h1>Настройки системы</h1>";
  page +="<table class='root'>";
  page +="<tr><td><div><a href='/outputStates' class='btn'>Состояние выходов</a></div></td></tr>";      
  page +="<tr><td><div><a href='/getAlarmSettings' class='btn'>Уставки управления выходами</a></div></td></tr>";
  page +="<tr><td><div><a href='/setOutputs' class='btn'>Режим управления выходами</a></div></td></tr>";  
  page +="<tr><td><div><a href='/display' class='btn'>Дисплей</a></div></td></tr>";  
  page +="<tr><td><div><a href='/setDatetime' class='btn'>Установка времени и даты</a></div></td></tr>";  
  page +="</table>";
  page +="</body></html>";
  return page;
}

