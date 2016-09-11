#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* D  */
void Application::NewOrderSingle( const char* sessionType )
{
  FIX44::NewOrderSingle message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
