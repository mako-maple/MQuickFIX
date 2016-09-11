#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* N  */
void Application::onMessage(const FIX44::ListStatus& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
