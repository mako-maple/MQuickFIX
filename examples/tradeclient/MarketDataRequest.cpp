#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* V  */
void Application::MarketDataRequest( 
    const char* sessionType 
  , const std::string& target
  , const int depth
  , const bool& aggregate
  , const FIX::SubscriptionRequestType& a_subType
)
{
  FIX44::MarketDataRequest message;

  /* 262  */ message.set(FIX::MDReqID( target ));
  /* 263  */ message.set(FIX::SubscriptionRequestType( a_subType ));
  /* 264  */ message.set(FIX::MarketDepth( depth )); /* 1=TopBook, 0=FullBook */
  /* 265  */ message.set(FIX::MDUpdateType( FIX::MDUpdateType_INCREMENTAL_REFRESH )); /* 1 Only */
  /* 266  */ message.set(FIX::AggregatedBook( aggregate )); /* true = Aggregate, false = NoAggregate */

  std::vector<char>c;
  c.push_back(FIX::MDEntryType_BID);                           /* 0 */
  c.push_back(FIX::MDEntryType_OFFER);                         /* 1 */
  if( depth )
  {
    c.push_back(FIX::MDEntryType_TRADING_SESSION_HIGH_PRICE);  /* 7 */
    c.push_back(FIX::MDEntryType_TRADING_SESSION_LOW_PRICE);   /* 8 */
    c.push_back(FIX::MDEntryType_TRADING_SESSION_VWAP_PRICE);  /* 9 */
  }

  while( !c.empty() )
  {
    /* 267  */ FIX44::MarketDataRequest::NoMDEntryTypes entryType;
    /* 269  */ entryType.set(FIX::MDEntryType( c.back() ));
    c.pop_back();
    message.addGroup( entryType );
  }

  /* 146  */ FIX44::MarketDataRequest::NoRelatedSym symbolGroup;
  /* 55   */ symbolGroup.set( FIX::Symbol( target ));
  message.addGroup( symbolGroup );

  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
}
