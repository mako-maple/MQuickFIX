#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AF */
void Application::OrderMassStatusRequest()
{
  FIX44::OrderMassStatusRequest message;
  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<AF> OrderMassStatusRequest: " << std::endl << message.toXML() << std::endl;
}
