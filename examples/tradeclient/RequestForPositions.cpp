#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AN */
void Application::RequestForPositions( const char* sessionType )
{
  FIX44::RequestForPositions message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
