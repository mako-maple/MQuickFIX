#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* r  */
void Application::onMessage(const FIX44::OrderMassCancelReport& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<r> OrderMassCancelReport:" << message.toXML() << std::endl;
}
