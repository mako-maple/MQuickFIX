#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* y  */
void Application::onMessage(const FIX44::SecurityList& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
