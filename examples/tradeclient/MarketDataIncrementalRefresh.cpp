#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* X  */
void Application::onMessage(const FIX44::MarketDataIncrementalRefresh& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<X> MarketDataIncrementalRefresh:" << message.toXML() << std::endl;
}
