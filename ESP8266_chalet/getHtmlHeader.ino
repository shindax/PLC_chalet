#include "system.h"

String getHtmlHeader()
{
  String page = "<!DOCTYPE html><html><head>";
  page += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>";
  page +="<title>Local network</title>";
  page += getHtmlStyle();
  page += "</head>";
  return page;
}
