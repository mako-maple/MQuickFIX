#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* x  */
void Application::SecurityListRequest( const char* sessionType )
{
  FIX44::SecurityListRequest message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
