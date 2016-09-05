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

#include "quickfix/fix44/TestRequest.h"                         // < 1 >
#include "quickfix/fix44/TradingSessionStatus.h"                // < h >
#include "quickfix/fix44/TradingSessionStatusRequest.h"         // < g >

#include <queue>
#include <map>

const char SessionTypeTrade [] = "Trade";
const char SessionTypeRatefeed [] = "Ratefeed";

class Application :
      public FIX::Application,
      public FIX::MessageCracker
{
public:
  Application
  ( const FIX::SessionSettings& settings )
    : m_settings( settings ) {}

  void run();

private:
  FIX::SessionSettings m_settings;
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

  /* h  */ void onMessage(const FIX44::TradingSessionStatus&, const FIX::SessionID& );
  /* 1  */ void TestRequest(const char* sessionType );

  void SetMessageHeader( FIX::Message& message, const char* sessionType );

};

#endif
