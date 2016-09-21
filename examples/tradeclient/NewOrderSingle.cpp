#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* D  */
void Application::NewOrderSingle()
{
  FIX44::NewOrderSingle message;
  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<D> NewOrderSingle: " << std::endl << message.toXML() << std::endl;
}
