#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* q  */
void Application::OrderMassCancelRequest()
{
  FIX44::OrderMassCancelRequest message;
  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<q> OrderMassCancelRequest: " << std::endl << message.toXML() << std::endl;
}
