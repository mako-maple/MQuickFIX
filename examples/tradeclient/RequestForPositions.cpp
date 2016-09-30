#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AN */
void Application::RequestForPositions( const int& reqType, const std::string& clearingBusinessDate )
{
  FIX44::RequestForPositions message;

  /* 710  */ message.set( FIX::PosReqID( "PosReq_" + YmdHMSs() ));
  /* 263  */ message.set( FIX::SubscriptionRequestType(FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES /* 1 */ ) );
  /* 724  */ message.set( FIX::PosReqType( reqType )); /* 0 Position, 1 Trade */

  /* 453  */ FIX44::RequestForPositions::NoPartyIDs parties;
  /* 448  */ parties.set(FIX::PartyID( m_partyID ));
  /* 452  */ parties.set(FIX::PartyRole( FIX::PartyRole_CLIENT_ID /* 3 */ ));
  message.addGroup( parties );

  /* 1    */ message.set( FIX::Account( m_accountID ));
  /* 581  */ message.set( FIX::AccountType( FIX::AccountType_ACCOUNT_IS_CARRIED_ON_CUSTOMER_SIDE_OF_BOOKS /* 1 */ ));
  /* 60   */ message.set( FIX::TransactTime() );

  if ( clearingBusinessDate != "" )
  /* 715  */ message.set( FIX::ClearingBusinessDate( clearingBusinessDate + "-00:00:00"));

  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<AN> RequestForPositions: " << std::endl << message.toXML() << std::endl;
}
