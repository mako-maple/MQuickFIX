#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AF */
void Application::OrderMassStatusRequest()
{
  FIX44::OrderMassStatusRequest message(
    /* 584  */ FIX::MassStatusReqID( "OrderMassStatusReq_" + YmdHMSs() ),
    /* 585  */ FIX::MassStatusReqType( FIX::MassStatusReqType_STATUS_FOR_ORDERS_FOR_A_PARTYID ) /* 8 only */
  );
  /* 60   */ message.set(FIX::TransactTime());

  /* 453  */ FIX44::OrderMassStatusRequest::NoPartyIDs party;
  /* 448  */ party.set(FIX::PartyID( m_partyID ));
  /* 452  */ party.set(FIX::PartyRole( FIX::PartyRole_CLIENT_ID )); /* 3 only */
  message.addGroup( party );

  SetMessageHeader( message );
  FIX::Session::sendToTarget( message );
  std::cout << "<AF> OrderMassStatusRequest: " << std::endl << message.toXML() << std::endl;
}
