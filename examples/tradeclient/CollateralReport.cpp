#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* BA  */
void Application::onMessage(const FIX44::CollateralReport& message, const FIX::SessionID& sessionID )
{
  std::cout << "<BA> CollateralReport: " << std::endl << message.toXML() << std::endl;
}
