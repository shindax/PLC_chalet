#include "system.h"

String getAlarmSettingsHtml()
{
  String page = getHtmlHeader();
  page +="<body>";
  page +="<h1>Уставки выходов</h1>";
  page +="<table class='settings'>";

  page +="<tr>";
  page +="<td><div><a href='/setAlarmSettingsBank1' class='btn'>Банк 1</a></div></td>";
  page +="<td><div><a href='/setAlarmSettingsBank2' class='btn'>Банк 2</a></div></td>";
  page +="</tr>";

  page +="<tr>";
  page +="<td><div><a href='/setAlarmSettingsBank3' class='btn'>Банк 3</a></div></td>";
  page +="<td><div><a href='/setAlarmSettingsBank4' class='btn'>Банк 4</a></div></td>";
  page +="</tr>";

  page +="</table>";
  page += "<button onclick='location.href=\"/\"' type='button'>Назад</button>";
  page +="</body></html>";
  return page;
}