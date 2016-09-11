#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* 9  */
void Application::onMessage(const FIX44::OrderCancelReject& message, const FIX::SessionID& sessionID )
{
  std::cout << message.toXML() << std::endl;
}
