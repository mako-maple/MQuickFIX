#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AP */
void Application::onMessage(const FIX44::PositionReport& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
