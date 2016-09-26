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

void Application::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID )
{
  if (FIX::MsgType_Logon == message.getHeader().getField(FIX::FIELD::MsgType))
  {
    const FIX::Dictionary& dic = m_settings.get( sessionID );
    if (dic.has("Password"))
      message.getHeader().setField(FIX::Password(dic.getString("Password")));
  }
}

void Application::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
  const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );

  if( msgTypeValue == "PU" )
    onMessage( (const FIX44::OrderRateUpdate&)message, sessionID );
  else
  if( msgTypeValue == "CG" )
    onMessage( (const FIX44::PartyDetailsListReport&)message, sessionID );
  else
    crack( message, sessionID );

  if( msgTypeValue != "X" )
    std::cout << std::endl << "IN: " << message << std::endl;
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

  std::cout << std::endl << "OUT: " << message << std::endl;
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
        << "AN) Request For Positions" << std::endl
        <<  std::endl
        << "Vj) MarketDataRequest USD/JPY TOP Aggr" << std::endl
        << "Ve) MarketDataRequest EUR/USD TOP Aggr" << std::endl
        <<  std::endl
        << "Vjq) MarketDataRequest USD/JPY STOP" << std::endl
        << "Veq) MarketDataRequest EUR/USD STOP" << std::endl
        <<  std::endl
        << "r) MarketData Rate List" << std::endl
        << "rJ) MarketData USD/JPY" << std::endl
        <<  std::endl
        << "Action: ";
      std::cin >> action;
                
      if ( action == "q" ) break;
      else if ( action == "test1" ) TestRequest( SessionTypeTrade );
      else if ( action == "test2" ) TestRequest( SessionTypeRatefeed );
      else if ( action == "symbol" ) SecurityListRequest();
      else if ( action == "AN" ) RequestForPositions();

      else if ( action == "Vj" ) MarketDataRequest( "USD/JPY", 1, true,  true );
      else if ( action == "Ve" ) MarketDataRequest( "EUR/USD", 1, true,  true );

      else if ( action == "Vjq" ) MarketDataRequest( "USD/JPY", 0, false,  false );
      else if ( action == "Veq" ) MarketDataRequest( "EUR/USD", 0, false,  false );

      else if ( action == "r" ) 
        for (auto symbol : m_symbol)
          std::cout << symbol.first << " " 
            << rate[symbol.second].Time << " "
            << rate[symbol.second].Symbol << " "
            << rate[symbol.second].Bid << "\t"
            << rate[symbol.second].Spred << "\t"
            << rate[symbol.second].Ask << "\t"
            << std::endl;

      else if ( action == "rJ" ) 
          std::cout << "USD/JPY" << " " 
            << rate[m_symbol["USD/JPY"]].Time << " "
            << rate[m_symbol["USD/JPY"]].Symbol << " "
            << rate[m_symbol["USD/JPY"]].Bid << "\t"
            << rate[m_symbol["USD/JPY"]].Spred << "\t"
            << rate[m_symbol["USD/JPY"]].Ask << "\t"
            << std::endl;
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
