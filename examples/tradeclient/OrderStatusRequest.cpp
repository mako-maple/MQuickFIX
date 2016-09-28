#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* H  */
void Application::OrderStatusRequest()
{
  FIX44::OrderStatusRequest message;
  
  /* 37   */ message.set(FIX::OrderID( "OPEN_ORDER" ));
  /* 11   */ message.set(FIX::ClOrdID( "OPEN_ORDER" ));
  /* 54   */ message.set(FIX::Side( FIX::Side_UNDISCLOSED ));
  /* 55   */ message.set(FIX::Symbol( "NA" ));

  /* 7559 */ message.set(FIX::OpenOrders( 'Y' ));

  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<H> OrderStatusRequest: " << std::endl << message.toXML() << std::endl;
}
