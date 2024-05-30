#include "system.h"

String displayHtml()
{
  String page = getHtmlHeader();
  page +="<body>";
  page +="<h1>Управление дисплеем</h1>";
  page += "<button class='display' onclick='location.href=\"/displayOn\"' type='button'>Включить</button>";
  page += "<button class='display' onclick='location.href=\"/displayOff\"' type='button'>Выключить</button>";
  page += "<button onclick='location.href=\"/\"' type='button'>Назад</button>";
  page +="</body></html>";
  return page;
}