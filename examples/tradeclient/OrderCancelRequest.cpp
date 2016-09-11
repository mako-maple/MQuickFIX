#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* F  */
void Application::OrderCancelRequest( const char* sessionType )
{
  FIX44::OrderCancelRequest message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
