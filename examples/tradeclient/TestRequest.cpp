#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

void Application::TestRequest( const char* sessionType )
{
  FIX44::TestRequest message((FIX::TestReqID)sessionType);
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
