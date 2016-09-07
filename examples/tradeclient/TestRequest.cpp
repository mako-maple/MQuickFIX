#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

void Application::TestRequest( const char* sessionType )
{
  FIX44::TestRequest message(
  /* 112  */ FIX::TestReqID("TestReqest_" + YmdHMSs())
  );
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
