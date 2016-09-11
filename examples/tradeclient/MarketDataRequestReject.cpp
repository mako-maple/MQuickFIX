#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* Y  */
void Application::onMessage(const FIX44::MarketDataRequestReject& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
