#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AO */
void Application::onMessage(const FIX44::RequestForPositionsAck& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
