#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* H  */
void Application::OrderStatusRequest()
{
  FIX44::OrderStatusRequest message;
  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<H> OrderStatusRequest: " << std::endl << message.toXML() << std::endl;
}
