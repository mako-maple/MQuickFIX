#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* G  */
void Application::OrderCancelReplaceRequest()
{
  FIX44::OrderCancelReplaceRequest message;
  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<G> OrderCancelReplaceRequest: " << std::endl << message.toXML() << std::endl;
}
