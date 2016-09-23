#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* j  */
void Application::onMessage(const FIX44::BusinessMessageReject& message, const FIX::SessionID& sessionID )
{
  std::cout << "<j> BusinessMessageReject: " << std::endl << message.toXML() << std::endl;
}
