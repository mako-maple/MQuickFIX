#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* V  */
void Application::MarketDataRequest( const char* sessionType )
{
  FIX44::MarketDataRequest message;
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
