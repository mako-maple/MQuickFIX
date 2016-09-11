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

#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

void Application::onLogon( const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "Logon - " << sessionID << std::endl;
}

void Application::onLogout( const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "Logout - " << sessionID << std::endl;
}

void Application::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
  crack( message, sessionID );
  std::cout << std::endl << "IN: " << message << std::endl;
}

void Application::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID )
{
  if (FIX::MsgType_Logon == message.getHeader().getField(FIX::FIELD::MsgType))
  {
    const FIX::Dictionary& dic = m_settings.get( sessionID );
    if (dic.has("Password"))
      message.getHeader().setField(FIX::Password(dic.getString("Password")));
  }
  
  if (FIX::MsgType_TestRequest == message.getHeader().getField(FIX::FIELD::MsgType))
    InsertTestRequest( message );
}

void Application::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::DoNotSend )
{
  try
  {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField( possDupFlag );
    if ( possDupFlag ) throw FIX::DoNotSend();
  }
  catch ( FIX::FieldNotFound& ) {}

  std::cout << std::endl
  << "OUT: " << message << std::endl;
}

void Application::run()
{
  while ( true )
  {
    try
    {
      std::string action;
      std::cout << std::endl
        << "q) Quit" << std::endl
        << "test1) testRequest Trade" << std::endl
        << "test2) testRequest Ratefeed" << std::endl
        << "symbol) Security List Request" << std::endl
        << "Action: ";
      std::cin >> action;
                
      if ( action == "q" ) break;
      else if ( action == "test1" ) TestRequest( SessionTypeTrade );
      else if ( action == "test2" ) TestRequest( SessionTypeRatefeed );
      else if ( action == "symbol" ) SecurityListRequest();
    }
    catch ( std::exception & e )
    {
      std::cout << "Message Not Sent: " << e.what();
    }
  }
}

void Application::SetMessageHeader( FIX::Message& message, const char* sessionType )
{
  auto itr = m_senderID.find(sessionType);
  if (itr == m_senderID.end())
  {
    const std::set <FIX::SessionID> session = m_settings.getSessions();
    for (std::set <FIX::SessionID>::iterator it = session.begin(); it != session.end(); ++it)
    {
      const FIX::Dictionary dic = m_settings.get( *it );
      if (dic.getString("SessionType") == sessionType)
      {
        m_senderID[dic.getString("SessionType")] = dic.getString("SenderCompID");
        m_targetID[dic.getString("SessionType")] = dic.getString("TargetCompID");
        break;
      }
    }
  }
  message.getHeader().setField((FIX::SenderCompID)m_senderID[sessionType]);
  message.getHeader().setField((FIX::TargetCompID)m_targetID[sessionType]);
}


FIX::MySQLConnection *Application::MySQLConnect()
{
  if (m_database == "")
  {
    m_database = AppDEFAULT_DATABASE;
    m_user = AppDEFAULT_USER;
    m_pass = AppDEFAULT_PASS;
    m_host = AppDEFAULT_HOST;
    m_port = AppDEFAULT_PORT;

    const FIX::Dictionary dic = m_settings.get();
    if (dic.has("MySQLStoreDatabase")) m_database = dic.getString("MySQLStoreDatabase");
    if (dic.has("MySQLStoreUser")) m_user = dic.getString("MySQLStoreUser");
    if (dic.has("MySQLStorePassword")) m_pass = dic.getString("MySQLStorePassword");
    if (dic.has("MySQLStoreHost")) m_host = dic.getString("MySQLStoreHost");
    if (dic.has("MySQLStorePort")) m_port = dic.getInt("MySQLStorePort");
  }

  return new FIX::MySQLConnection( m_database, m_user, m_pass, m_host, m_port );
}

std::string Application::getSetting( const char* key )
{
  std::string ret = "";
  const FIX::Dictionary dic = m_settings.get();
  if (dic.has(key)) ret = dic.getString(key);
  return key;
}

std::string Application::YmdHMSs()
{
  FIX::UtcTimeStamp time;
 int year, month, day, hour, minute, second, millis;
 time.getYMD( year, month, day );
 time.getHMS( hour, minute, second, millis );
 char tm[ 20 ];
 // YYYYmmdd-HHMMSS.sss
 // 1234567890123456789
 STRING_SPRINTF( tm, "%04d%02d%02d-%02d%02d%02d.%03d", year, month, day, hour, minute, second, millis );
 return tm;
}
