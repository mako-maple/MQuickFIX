#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* 8  */
void Application::onMessage(const FIX44::ExecutionReport& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
