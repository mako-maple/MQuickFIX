#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AF */
void Application::OrderMassStatusRequest( const char* sessionType )
{
  FIX44::OrderMassStatusRequest message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
