#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* q  */
void Application::OrderMassCancelRequest( const char* sessionType )
{
  FIX44::OrderMassCancelRequest message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
