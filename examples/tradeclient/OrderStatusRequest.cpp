#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* H  */
void Application::OrderStatusRequest( const char* sessionType )
{
  FIX44::OrderStatusRequest message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
