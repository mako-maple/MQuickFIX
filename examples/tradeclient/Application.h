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

#include "quickfix/fix44/TestRequest.h"                         // < 1 >
#include "quickfix/fix44/TradingSessionStatus.h"                // < h >
#include "quickfix/fix44/TradingSessionStatusRequest.h"         // < g >

#include <queue>
#include <map>
#include <syslog.h>

const char SessionTypeTrade [] = "Trade";
const char SessionTypeRatefeed [] = "Ratefeed";
const char DEFAULT_DATABASE[] = "quickfix";
const char DEFAULT_USER[] = "root";
const char DEFAULT_PASS[] = "";
const char DEFAULT_HOST[] = "localhost";
const short DEFAULT_PORT = 3306;

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
  }

  ~Application()
  { if (m_sql) delete m_sql; }

  void run();

private:
  FIX::SessionSettings m_settings;
  FIX::MySQLConnection *m_sql;
  std::string m_database;
  std::string m_user;
  std::string m_pass;
  std::string m_host;
  short m_port;
  std::map<std::string, std::string> m_senderID;
  std::map<std::string, std::string> m_targetID;

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

  /* h  */ void onMessage( const FIX44::TradingSessionStatus&, const FIX::SessionID& );
  /* 1  */ void TestRequest( const char* );
  /* 1  */ void InsertTestRequest( FIX::Message& );

  void SetMessageHeader( FIX::Message&, const char* );
  FIX::MySQLConnection *MySQLConnect();
  std::string YmdHMSs();
};

#endif
