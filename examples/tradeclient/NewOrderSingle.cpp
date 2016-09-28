#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* D  */
void Application::NewOrderSingle(
  /* 40   */ const char &ordType,
  /* 54   */ const char &side,
  /* 55   */ const std::string &symbol,
  /* 38   */ const long &qty,
  /* 44   */ const double &price,
  /* 526  */ const std::string &secClOrdID
)
{
  FIX44::NewOrderSingle message(
    /* 11   */ FIX::ClOrdID( YmdHMSs() + symbol ),
    /* 54   */ FIX::Side( side ),
    /* 60   */ FIX::TransactTime(),
    /* 40   */ FIX::OrdType( ordType )
  );

  if (! secClOrdID.empty() )
    /* 526  */ message.set(FIX::SecondaryClOrdID( secClOrdID ));

  /* 21   */ message.set(FIX::HandlInst(FIX::HandlInst_AUTOMATED_EXECUTION_ORDER_PRIVATE_NO_BROKER_INTERVENTION)); /* 1 only */
  /* 460  */ message.set(FIX::Product(FIX::Product_CURRENCY)); /* 4 only */

  /* 59   */ // message.set(FIX::TimeInForce(FIX::TimeInForce_DAY));                     /* 0  DAY */
  /* 59   */ message.set(FIX::TimeInForce(FIX::TimeInForce_GOOD_TILL_CANCEL));           /* 1  GTC */
  /* 59   */ // message.set(FIX::TimeInForce(FIX::TimeInForce_IMMEDIATE_OR_CANCEL));     /* 3  IOC */
  /* 59   */ // message.set(FIX::TimeInForce(FIX::TimeInForce_FILL_OR_KILL));            /* 4  FOK */

  /* 59   */ // message.set(FIX::TimeInForce(FIX::TimeInForce_GOOD_TILL_DATE));          /* 6  GTD */
  /* 432  */ // message.set(FIX::ExpireDate("YYYYMMDD"));                                /* is GTD set Expire Date */
  /* 126  */ // message.set(FIX::ExpireTime("YYYYMMDD-HH:MM:SS"));                       /* is GTD set Expire Time */

  /* 15   */ message.set(FIX::Currency(symbol.substr(0,3)));
  /* 55   */ message.set(FIX::Symbol(symbol));
  /* 38   */ message.set(FIX::OrderQty(qty));
  /* 110  */ // message.set(FIX::MinQty(qty));

  switch( ordType )
  {
    case FIX::OrdType_FOREX_F :
      /* 44   */ message.set(FIX::Price(price));
  }

  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<D> NewOrderSingle: " << std::endl << message.toXML() << std::endl;
}
