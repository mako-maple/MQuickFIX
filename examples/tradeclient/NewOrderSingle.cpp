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
  /* 59   */ const char &timeInForce,
  /* 44   */ const double &price,
  /* 99   */ // const double &stopPrice,
  /* 662  */ const double &slip,
  /* 526  */ const bool &closingPositions
)
{
  FIX44::NewOrderSingle message(
    /* 11   */ FIX::ClOrdID( YmdHMSs() + symbol ),
    /* 54   */ FIX::Side( side ),
    /* 60   */ FIX::TransactTime(),
    /* 40   */ FIX::OrdType( ordType )
  );
  /* 21   */ message.set(FIX::HandlInst(FIX::HandlInst_AUTOMATED_EXECUTION_ORDER_PRIVATE_NO_BROKER_INTERVENTION)); /* 1 only */
  /* 460  */ message.set(FIX::Product(FIX::Product_CURRENCY)); /* 4 only */

  /* 15   */ message.set(FIX::Currency(symbol.substr(0,3)));
  /* 55   */ message.set(FIX::Symbol(symbol));
  /* 38   */ message.set(FIX::OrderQty(qty));
  /* 110  */ // message.set(FIX::MinQty(qty));

  if ( closingPositions )
    /* 526  */ message.set(FIX::SecondaryClOrdID( "2" ));

  switch( timeInForce )
  {
    case FIX::TimeInForce_DAY :
      /* 59   */ message.set(FIX::TimeInForce(FIX::TimeInForce_DAY));                     /* 0  DAY */
      break;

    case FIX::TimeInForce_IMMEDIATE_OR_CANCEL :
      /* 59   */ message.set(FIX::TimeInForce(FIX::TimeInForce_IMMEDIATE_OR_CANCEL));     /* 3  IOC */
      break;
    
    case FIX::TimeInForce_FILL_OR_KILL :
      /* 59   */ message.set(FIX::TimeInForce(FIX::TimeInForce_FILL_OR_KILL));            /* 4  FOK */
      break;

    //case FIX::TimeInForce_GOOD_TILL_DATE :
      /* 59   */ // message.set(FIX::TimeInForce(FIX::TimeInForce_GOOD_TILL_DATE));          /* 6  GTD */
      /* 432  */ // message.set(FIX::ExpireDate("YYYYMMDD"));                                /* is GTD set Expire Date */
      /* 126  */ // message.set(FIX::ExpireTime("YYYYMMDD-HH:MM:SS"));                       /* is GTD set Expire Time */
      break;

    default:
      /* 59   */ message.set(FIX::TimeInForce(FIX::TimeInForce_GOOD_TILL_CANCEL));           /* 1  GTC */
  }

  switch( ordType )
  {
    /* T */ case FIX::OrdType_BENCHMARK :
      /* 662  */ message.set(FIX::BenchmarkPrice(slip));
      /* 44   */ message.set(FIX::Price(price));
      break;

    /* 3 */ case FIX::OrdType_STOP :
      /* 99   */ // message.set(FIX::StopPx(stopPrice));
      break;

    /* 4 */ case FIX::OrdType_STOP_LIMIT :
      /* 44   */ message.set(FIX::Price(price));
      /* 99   */ // message.set(FIX::StopPx(stopPrice));
      break;

    /* F */ case FIX::OrdType_FOREX_F :
      /* 44   */ message.set(FIX::Price(price));
      break;
  }

  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<D> NewOrderSingle: " << std::endl << message.toXML() << std::endl;
}

void Application::Order_Limit(
  /* 54   */ const char &side,
  /* 55   */ const std::string &symbol,
  /* 38   */ const long &qty,
  /* 44   */ double price,
  /* 59   */ char timeInForce,
  /* 526  */ const bool &closingPositions
)
{
  if ( price == 0 )
  {
    if ( side == FIX::Side_BUY )
      price = rate[m_symbol[symbol]].Ask;
    else
      price = rate[m_symbol[symbol]].Bid;
  }

  if ( timeInForce == 0x00 )
    timeInForce = FIX::TimeInForce_FILL_OR_KILL;

  NewOrderSingle(
    /* 40   */ FIX::OrdType_FOREX_F,
    /* 54   */ side,
    /* 55   */ symbol,
    /* 38   */ qty,
    /* 59   */ timeInForce,
    /* 44   */ price,
    /* 526  */ closingPositions
  );
}
