#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* G  */
void Application::OrderCancelReplaceRequest( const char* sessionType )
{
  FIX44::OrderCancelReplaceRequest message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
