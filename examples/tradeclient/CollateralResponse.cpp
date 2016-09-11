#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AZ  */
void Application::onMessage(const FIX44::CollateralResponse& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<AZ> CollateralResponse:" << message.toXML() << std::endl;
}
