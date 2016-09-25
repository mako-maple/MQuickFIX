#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* V  */
void Application::MarketDataRequest( 
    const std::string& target
  , const int depth
  , const bool& aggregate
  , const bool& subType
)
{
  FIX44::MarketDataRequest message;

  /* 262  */ message.set(FIX::MDReqID( target ));
  /* 264  */ message.set(FIX::MarketDepth( depth )); /* 1=TopBook, 0=FullBook */
  /* 265  */ message.set(FIX::MDUpdateType( FIX::MDUpdateType_INCREMENTAL_REFRESH )); /* 1 Only */
  /* 266  */ message.set(FIX::AggregatedBook( aggregate )); /* true = Aggregate, false = NoAggregate */

  if ( subType )
    /* 263  */ message.set(FIX::SubscriptionRequestType( FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES ));
  else
    /* 263  */ message.set(FIX::SubscriptionRequestType( FIX::SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST ));

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

  SetMessageHeader( message, SessionTypeRatefeed );
  InsertMarketDataRequest( message );
  FIX::Session::sendToTarget( message );

//  std::cout << "<V> MarketDataRequest: " << std::endl << message.toXML() << std::endl;
}

void Application::InsertMarketDataRequest( const FIX::Message& message )
{
  std::ostringstream s;

  /* 262  */ FIX::FieldBase reqID(FIX::FIELD::MDReqID, "");
  /* 263  */ FIX::FieldBase reqType(FIX::FIELD::SubscriptionRequestType, "1");
  /* 264  */ FIX::FieldBase depth(FIX::FIELD::MarketDepth, "1");
  /* 266  */ FIX::FieldBase aggregate(FIX::FIELD::AggregatedBook, "Y");
  /* 267  */ FIX::FieldBase entryTypesum(FIX::FIELD::NoMDEntryTypes, "0");

  /* 262  */ message.getFieldIfSet(reqID);
  /* 263  */ message.getFieldIfSet(reqType);
  /* 264  */ message.getFieldIfSet(depth);
  /* 266  */ message.getFieldIfSet(aggregate);
  /* 267  */ message.getFieldIfSet(entryTypesum);

  /* STOP Request */
  if( *reqType.getString().c_str() == FIX::SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST )
  {
    s <<
      "UPDATE `market_data_request` SET " <<
      "   `ReqType`   = '" << reqType << "' " <<
      " , `StopTime`  = now()               " <<
      " WHERE `ReqID` = '" << reqID   << "' ";

    FIX::MySQLQuery q( s.str() );
    m_sql->execute( q );
    return;
  }

  /* 146  */ FIX44::MarketDataRequest::NoRelatedSym symbolGroup;
  /* 55   */ FIX::FieldBase symbol(FIX::FIELD::Symbol, "NA");
  message.getGroup( 1, symbolGroup );
  symbolGroup.getFieldIfSet(symbol);

  s << "INSERT INTO `market_data_request` SET " <<
  /*-262  */  "   `ReqID`        = '" << reqID        << "' " <<
  /*-263  */  " , `ReqType`      = '" << reqType      << "' " <<
  /*-264  */  " , `Depth`        = '" << depth        << "' " <<
  /*-266  */  " , `Aggregate`    = '" << aggregate    << "' " <<
  /*-267  */  " , `EntryTypeSum` = '" << entryTypesum << "' " <<
  /*-55   */  " , `Symbol`       = '" << symbol       << "' " <<
  /*---   */  " , `StartTime`    = now() ";

  s << " ON DUPLICATE KEY UPDATE " <<
    "  `ReqType`       =  VALUES(`ReqType`) " <<
    " ,`Symbol`        =  VALUES(`Symbol`) " <<
    " ,`Depth`         =  VALUES(`Depth`) " <<
    " ,`Aggregate`     =  VALUES(`Aggregate`) " <<
    " ,`EntryTypeSum`  =  VALUES(`EntryTypeSum`) " <<
    " ,`RespDateTime`  =  NULL " <<
    " ,`Entries`       =  NULL " <<
    " ,`RejectReason`  =  NULL " <<
    " ,`RejectText`    =  NULL " <<
    " ,`StartTime`     =  now() " <<
    " ,`StopTime`      =  NULL ";

  FIX::MySQLQuery q( s.str() );
  m_sql->execute( q );
}
