#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* x  */
void Application::SecurityListRequest()
{
  FIX44::SecurityListRequest message(
  /* 320  */ FIX::SecurityReqID( "SecListReq_" + YmdHMSs() ),
  /* 559  */ FIX::SecurityListRequestType( FIX::SecurityListRequestType_SYMBOL /* 0 */ )
  );
  InsertSecurityListRequest( message );
  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
//  std::cout << "<x> SecurityListRequest: " << std::endl << message.toXML() << std::endl;
}

void Application::InsertSecurityListRequest( const FIX::Message & message )
{
  /* 320  */ FIX::FieldBase reqID( FIX::FIELD::SecurityReqID, "0" );
  /* 320  */ message.getFieldIfSet( reqID );

  std::ostringstream s;
  s << "INSERT INTO `security_list_request` " <<
    "( `ReqID` ) " <<
    "VALUES " <<
    "( '" << reqID << "' )";
  
  FIX::MySQLQuery q( s.str() );
  m_sql->execute( q );
}
