#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AP */
void Application::onMessage(const FIX44::PositionReport& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<AP> PositionReport:" << message.toXML() << std::endl;
}
