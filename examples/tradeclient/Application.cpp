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
        << "Action: ";
      std::cin >> action;
                
      if ( action == "q" ) break;
      else if ( action == "test1" ) TestRequest( SessionTypeTrade );
      else if ( action == "test2" ) TestRequest( SessionTypeRatefeed );
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
