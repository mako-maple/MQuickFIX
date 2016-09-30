#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* h  */
void Application::onMessage(const FIX44::TradingSessionStatus& message, const FIX::SessionID& sessionID )
{
  // std::cout << std::endl << "<h> TradingSessionStatus:" << message.toXML() << std::endl;

  switch( std::stoi(message.getField(FIX::FIELD::TradSesStatus)) )
  {
    case FIX::TradSesStatus_OPEN :
//      SecurityListRequest();
      break;
    
    case FIX::TradSesStatus_CLOSED :
    case FIX::TradSesStatus_PRE_CLOSE :
      std::cout << std::endl << "<h> TradingSessionStatus:" << message.toXML() << std::endl;
      break;
  }
}
