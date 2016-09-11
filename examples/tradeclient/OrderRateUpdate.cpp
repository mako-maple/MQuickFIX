#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* MsgType  PU  */
void Application::onMessage( const FIX44::OrderRateUpdate& message, const FIX::SessionID& sessionID )
{
  std::cout << "<PU> OrderRateUpdate: " << std::endl << message.toXML() << std::endl;
}
