#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* h  */
void Application::onMessage(const FIX44::TradingSessionStatus& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<h> TradingSessionStatus:" << message.toXML() << std::endl;
}
