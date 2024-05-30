#include "system.h"

String getOutputStateHtml( unsigned char mode, unsigned char state )
{
  String stateClass = "" ;
  switch( mode ){
    case 2:
            if( state )
              stateClass = "auto-on";
                else
                  stateClass = "auto-off";
            break;
    case 1:
            stateClass="manual-on";
            break;
    case 0:
            stateClass="manual-off";
            break;
    default: 
            stateClass = "auto-off";
            break;
  }
  return stateClass;
}

String outputStatesHtml()
{
  String page = getHtmlHeader();
  page +="<body>";
  page +="<h1>Состояние выходов</h1>";

  String state0 = getOutputStateHtml( usartBuf[0], usartBuf[4] );
  String state1 = getOutputStateHtml( usartBuf[1], usartBuf[5] );
  String state2 = getOutputStateHtml( usartBuf[2], usartBuf[6] );
  String state3 = getOutputStateHtml( usartBuf[3], usartBuf[7] );

  page +="<div class='outputs'>";
    page +="<div class='"+ state0 +"'>1</div>";
    page +="<div class='"+ state1 +"'>2</div>";
    page +="<div class='"+ state2 +"'>3</div>";
    page +="<div class='"+ state3 +"'>4</div>";        
  page +="</div>";

  page += "<button onclick='location.href=\"/\"' type='button'>Назад</button>";
  page +="</body></html>";
  return page;
}
