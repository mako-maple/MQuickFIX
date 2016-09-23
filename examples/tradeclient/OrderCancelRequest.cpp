#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* F  */
void Application::OrderCancelRequest()
{
  FIX44::OrderCancelRequest message;
  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<F> OrderCancelRequest: " << std::endl << message.toXML() << std::endl;
}
