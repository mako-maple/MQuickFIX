#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AZ  */
void Application::onMessage(const FIX44::CollateralResponse& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
