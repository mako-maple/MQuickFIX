/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifndef TRADECLIENT_APPLICATION_H
#define TRADECLIENT_APPLICATION_H

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"
#include "quickfix/MySQLConnection.h"

#include "quickfix/fix44/TestRequest.h"                     // < 1 >
#include "quickfix/fix44/OrderMassCancelRequest.h"          // < q >
#include "quickfix/fix44/SecurityListRequest.h"             // < x >
#include "quickfix/fix44/NewOrderSingle.h"                  // < D >
#include "quickfix/fix44/OrderCancelRequest.h"              // < F >
#include "quickfix/fix44/OrderCancelReplaceRequest.h"       // < G >
#include "quickfix/fix44/OrderStatusRequest.h"              // < H >
#include "quickfix/fix44/MarketDataRequest.h"               // < V >
#include "quickfix/fix44/OrderMassStatusRequest.h"          // < AF >
#include "quickfix/fix44/RequestForPositions.h"             // < AN >

#include "quickfix/fix44/ExecutionReport.h"                 // < 8 >
#include "quickfix/fix44/OrderCancelReject.h"               // < 9 >
#include "quickfix/fix44/TradingSessionStatus.h"            // < h >
#include "quickfix/fix44/BusinessMessageReject.h"           // < j >
#include "quickfix/fix44/OrderMassCancelReport.h"           // < r >
#include "quickfix/fix44/SecurityList.h"                    // < y >
#include "quickfix/fix44/Email.h"                           // < C >
#include "quickfix/fix44/ListStatus.h"                      // < N >
#include "quickfix/fix44/MarketDataIncrementalRefresh.h"    // < X >
#include "quickfix/fix44/MarketDataRequestReject.h"         // < Y >
#include "quickfix/fix44/RequestForPositionsAck.h"          // < AO >
#include "quickfix/fix44/PositionReport.h"                  // < AP >
#include "quickfix/fix44/CollateralResponse.h"              // < AZ >
#include "quickfix/fix44/CollateralReport.h"                // < BA >

#include "PartyDetailsListReport.h"                         // < CG >
#include "OrderRateUpdate.h"                                // < PU >

#include "Rate.h"

#include <queue>
#include <vector>
#include <map>
#include <syslog.h>

const char SessionTypeTrade [] = "Trade";
const char SessionTypeRatefeed [] = "Ratefeed";
const char AppDEFAULT_DATABASE[] = "quickfix";
const char AppDEFAULT_USER[] = "root";
const char AppDEFAULT_PASS[] = "";
const char AppDEFAULT_HOST[] = "localhost";
const short AppDEFAULT_PORT = 3306;

class Application :
      public FIX::Application,
      public FIX::MessageCracker
{
public:
  Application
  ( const FIX::SessionSettings& settings )
    : m_settings( settings ) 
  {
    openlog("mQuickFIX", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    m_sql = MySQLConnect();
    m_accountID = getSetting("AccountID");
    m_partyID = getSetting("PartyID");
    m_tradingSessionStatus = 0;
  }

  ~Application()
  { 
    if (m_sql) delete m_sql; 
  }

  void run();

private:
  FIX::SessionSettings m_settings;
  FIX::MySQLConnection *m_sql;
  std::string m_database;
  std::string m_user;
  std::string m_pass;
  std::string m_host;
  short m_port;
  short m_tradingSessionStatus;
  std::map<std::string, std::string> m_senderID;
  std::map<std::string, std::string> m_targetID;
  std::string m_accountID;
  std::string m_partyID;

  std::map<std::string, int> m_symbol;
  std::vector<Rate> rate;

  void onCreate( const FIX::SessionID& ) {}
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message&, const FIX::SessionID& );
  void toApp( FIX::Message&, const FIX::SessionID& )
    throw( FIX::DoNotSend );
  void fromAdmin( const FIX::Message&, const FIX::SessionID& )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );


  /* 8  */ void onMessage( const FIX44::ExecutionReport&,              const FIX::SessionID& );
  /* 9  */ void onMessage( const FIX44::OrderCancelReject&,            const FIX::SessionID& );
  /* h  */ void onMessage( const FIX44::TradingSessionStatus&,         const FIX::SessionID& );
  /* j  */ void onMessage( const FIX44::BusinessMessageReject&,        const FIX::SessionID& );
  /* r  */ void onMessage( const FIX44::OrderMassCancelReport&,        const FIX::SessionID& );
  /* y  */ void onMessage( const FIX44::SecurityList&,                 const FIX::SessionID& );
  /* C  */ void onMessage( const FIX44::Email&,                        const FIX::SessionID& );
  /* N  */ void onMessage( const FIX44::ListStatus&,                   const FIX::SessionID& );
  /* X  */ void onMessage( const FIX44::MarketDataIncrementalRefresh&, const FIX::SessionID& );
           void InsertMarketData( const FIX::Message& );
  /* Y  */ void onMessage( const FIX44::MarketDataRequestReject&,      const FIX::SessionID& );
  /* AO */ void onMessage( const FIX44::RequestForPositionsAck&,       const FIX::SessionID& );
  /* AP */ void onMessage( const FIX44::PositionReport&,               const FIX::SessionID& );
  /* AZ */ void onMessage( const FIX44::CollateralResponse&,           const FIX::SessionID& );
  /* BA */ void onMessage( const FIX44::CollateralReport&,             const FIX::SessionID& );

  /* CG */ void onMessage( const FIX44::PartyDetailsListReport&,       const FIX::SessionID& );
  /* PU */ void onMessage( const FIX44::OrderRateUpdate&,              const FIX::SessionID& );


  /* 1  */ void TestRequest( const char* );
           void InsertTestRequest( FIX::Message& );
  /* q  */ void OrderMassCancelRequest();
  /* x  */ void SecurityListRequest();
           void InsertSecurityListRequest( const FIX::Message& );
  /* D  */ void NewOrderSingle(
                  /* 40   OrderType */ const char&,
                  /* 54   Side */ const char&,
                  /* 55   Symbol */ const std::string&,
                  /* 38   Quantity */ const long& = 10000,
                  /* 44   Price */ const double& = 0.0,
                  /* 662  Slip */ const double& = 0.0,
                  /* 526  SecondaryClOrdID */ const std::string& = ""  
                );
  /* F  */ void OrderCancelRequest();
  /* G  */ void OrderCancelReplaceRequest();
  /* H  */ void OrderStatusRequest();
  /* V  */ void MarketDataRequest( const std::string&, int, const bool&, const bool& );
           void InsertMarketDataRequest( const FIX::Message& );
  /* AF */ void OrderMassStatusRequest();
  /* AN */ void RequestForPositions( const int& pos = FIX::PosReqType_TRADES, const std::string& clearingBusinessDate = "" );

  void SetMessageHeader( FIX::Message&, const char* = SessionTypeTrade );
  FIX::MySQLConnection *MySQLConnect();
  std::string getSetting( const char* );
  std::string YmdHMSs();
};

#endif
