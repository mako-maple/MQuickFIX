#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* C  */
void Application::onMessage(const FIX44::Email& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<C> Email:" << message.toXML() << std::endl;
}
