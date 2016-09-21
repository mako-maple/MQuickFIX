#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* 1  */
void Application::TestRequest( const char* sessionType )
{
  FIX44::TestRequest message(
  /* 112  */ FIX::TestReqID("TestReqest_" + YmdHMSs())
  );
  SetMessageHeader( message, sessionType );
  FIX::Session::sendToTarget( message );
  std::cout << "<1> TestRequest: " << std::endl << message.toXML() << std::endl;
}

/* 1  */ 
void Application::InsertTestRequest( FIX::Message& message )
{
  /* 112  */ FIX::FieldBase testReqID(FIX::FIELD::TestReqID, "0");
  /* 49   */ FIX::FieldBase sender(FIX::FIELD::SenderCompID, "0");
  /* 55   */ FIX::FieldBase symbol(FIX::FIELD::Symbol, "0");

  /* 112  */ message.getFieldIfSet(testReqID);
  /* 49   */ message.getHeader().getFieldIfSet(sender);
  /* 55   */ message.getFieldIfSet(symbol);

  // OPEN SQL Connection
  FIX::MySQLConnection *sql = Application::MySQLConnect();

  // INSERT
  std::ostringstream s1;
  s1 << "INSERT INTO `test_request` " <<
    "( " <<
    "   `TestReqID` " <<
    "  ,`SenderCompID` " <<
    ") VALUES " <<
    "( " <<
    "   '" << testReqID << "'" <<
    "  ,'" << sender << "'" <<
    ") ON DUPLICATE KEY UPDATE `TestReqID` = '" << testReqID << "'";

  // Insert Message 
  FIX::MySQLQuery q1( s1.str() );
  sql->execute( q1 );

  // Get AUTO_INCREMENT  ID
  FIX::MySQLQuery q2( "SELECT @@identity AS id" );
  sql->execute( q2 );

  // SELECT
  std::ostringstream s2;
  s2 << "SELECT `id`, `TestReqID`, `SenderCompID`, `Created_at` " <<
    "      FROM `test_request`" <<
    "     WHERE `id` = '" << q2.getValue(0,0) << "'";

  // SELECT Insert Record
  FIX::MySQLQuery q3( s2.str() );
  sql->execute( q3 );

  // echo Insert Record
  std::cout << 
    "TEST_REQUEST - row - " << q3.rows() << std::endl <<
    "              `id`       :" << q3.getValue(0,0) << std::endl <<
    "              `TestReqID`:" << q3.getValue(0,1) << std::endl <<
    "              `SenderID` :" << q3.getValue(0,2) << std::endl <<
    "              `Created`  :" << q3.getValue(0,3) << std::endl;

  // CLOSE SQL Connection
  delete sql;
}
