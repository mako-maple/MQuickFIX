#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* Y  */
void Application::onMessage(const FIX44::MarketDataRequestReject& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<Y> MarketDataRequestReject:" << message.toXML() << std::endl;

  /* init message data */
  /* 52   */ FIX::FieldBase respDateTime(FIX::FIELD::SendingTime, "");
  /* 262  */ FIX::FieldBase reqID(FIX::FIELD::MDReqID, "NA");
  /* 281  */ FIX::FieldBase rejectReason(FIX::FIELD::MDReqRejReason, "");
  /* 58   */ FIX::FieldBase rejectText(FIX::FIELD::Text, "");

  /* get message data */
  /* 52  */ message.getHeader().getFieldIfSet(respDateTime);
  /* 262 */ message.getFieldIfSet(reqID);
  /* 281 */ message.getFieldIfSet(rejectReason);
  /* 58  */ message.getFieldIfSet(rejectText);

  /* Reject Request is stopped ? */
  std::ostringstream s;
  s << "SELECT `Symbol` " <<
    "      FROM `market_data_request` " <<
    "     WHERE `ReqID` = '" << reqID << "' " <<
    "       AND `ReqType` = '" << FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES << "' ";

  FIX::MySQLQuery q( s.str() );
  m_sql->execute( q );

  /* UPDATE SQL */
  std::ostringstream s2;
  s2 <<  "UPDATE `market_data_request` SET " <<
    /*-281  */  "   `RejectReason`  = '" << rejectReason  << "' " <<
    /*-58   */  " , `RejectText`    = '" << rejectText    << "' " <<
    /*-52   */  " , `RespDateTime`  = '" << respDateTime  << "' ";

  /* not stopped - STOP SQL */
  if( q.rows() != 0 ) 
  {
    s2 <<
    /*-263  */  " , `ReqType`       = '" << FIX::SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST << "' " <<
    /*---   */  " , `StopTime`      = now() ";
  }

  /* DB Update */
  s2 << " WHERE `ReqID` = '" << reqID << "' ";
  FIX::MySQLQuery q2( s2.str() );
  m_sql->execute( q2 );

  /* not stopped - STOP request */
  if ( q.rows() != 0 ) 
  {
    if ( rate[ m_symbol[q.getValue(0,0)] ].feedStatus )
    {
      MarketDataRequest( 
      /* 55  Symbol      */ q.getValue(0,0),
      /* 264 Depth       */ 0,
      /* 266 Aggregate   */ false,
      /* 263 RequestType */ false
      );
    }
  }
}
