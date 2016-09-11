#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* MsgType  CG  */
void Application::onMessage( const FIX44::PartyDetailsListReport& message, const FIX::SessionID& sessionID )
{
  std::cout << "<CG> PartyDetailsListReport: " << std::endl << message.toXML() << std::endl;
}
