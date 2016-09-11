#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* C  */
void Application::onMessage(const FIX44::Email& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
