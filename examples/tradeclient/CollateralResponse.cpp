#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* AZ  */
void Application::onMessage(const FIX44::CollateralResponse& message, const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "<AZ> CollateralResponse:" << message.toXML() << std::endl;

  /* init message data */
  /* 52   */ FIX::FieldBase respDateTime(FIX::FIELD::SendingTime, "");
  /* 894  */ FIX::FieldBase reqID(FIX::FIELD::CollReqID, "0");
  /* 904  */ FIX::FieldBase respID(FIX::FIELD::CollRespID, "0");
  /* 905  */ FIX::FieldBase result(FIX::FIELD::CollAsgnRespType, "0");
  /* 60   */ FIX::FieldBase transactTime(FIX::FIELD::TransactTime, "");
  /* 291  */ FIX::FieldBase status(FIX::FIELD::FinancialStatus, "0");
  /* 1    */ FIX::FieldBase accountID(FIX::FIELD::Account, "0");
  /* 581  */ FIX::FieldBase accountType(FIX::FIELD::AccountType, "0");
  /* 15   */ FIX::FieldBase currency(FIX::FIELD::Currency, "NA");
  /* 898  */ FIX::FieldBase ratio(FIX::FIELD::MarginRatio, "0");
  /* 921  */ FIX::FieldBase startCash(FIX::FIELD::StartCash, "0");
  /* 922  */ FIX::FieldBase endCash(FIX::FIELD::EndCash, "0");
  /* 58   */ FIX::FieldBase text(FIX::FIELD::Text, "");

  /* get message data */
  /* 52   */ message.getHeader().getFieldIfSet( respDateTime );
  /* 894  */ message.getFieldIfSet( reqID );
  /* 904  */ message.getFieldIfSet( respID );
  /* 905  */ message.getFieldIfSet( result );
  /* 60   */ message.getFieldIfSet( transactTime );
  /* 291  */ message.getFieldIfSet( status );
  /* 1    */ message.getFieldIfSet( accountID );
  /* 581  */ message.getFieldIfSet( accountType );
  /* 15   */ message.getFieldIfSet( currency );
  /* 898  */ message.getFieldIfSet( ratio );
  /* 921  */ message.getFieldIfSet( startCash );
  /* 922  */ message.getFieldIfSet( endCash );
  /* 58   */ message.getFieldIfSet( text );

  /* get group */
  /* 898  */ FIX44::CollateralResponse::NoPartyIDs p;
  /* 448  */ FIX::FieldBase partyID(FIX::FIELD::PartyID, "0");
  /* 452  */ FIX::FieldBase partyRole(FIX::FIELD::PartyRole, "0");
  if (message.hasGroup(1, p))
  {
    message.getGroup(1, p);
    p.getFieldIfSet( partyID );
    p.getFieldIfSet( partyRole );
  }

  /* Insert Collateral Responce - balance_hist */
  std::ostringstream s;
  s << "INSERT INTO `balance_hist` " <<
    "( " <<
      "  `ReqID`          " <<  /* 710,894 */
      " ,`RespID`         " <<  /* 904  */
      " ,`Result`         " <<  /* 905  */
      " ,`RespDateTime`   " <<  /* 52   */
      " ,`TransactTime`   " <<  /* 60   */
      " ,`Status`         " <<  /* 291  */
      " ,`PartyID`        " <<  /* 448  */
      " ,`PartyRole`      " <<  /* 452  */
      " ,`AccountID`      " <<  /* 1    */
      " ,`AccountType`    " <<  /* 581  */
      " ,`Currency`       " <<  /* 15   */
      " ,`Ratio`          " <<  /* 898  */
      " ,`StartCash`      " <<  /* 921  */
      " ,`EndCash`        " <<  /* 922  */
      " ,`Text`           " <<  /* 58   */
    ") VALUES ("            <<
      ",'" << reqID         << "'" <<  /* 894  */
      ",'" << respID        << "'" <<  /* 904  */
      ",'" << result        << "'" <<  /* 905  */
      ",'" << respDateTime  << "'" <<  /* 52   */
      ",'" << transactTime  << "'" <<  /* 60   */
      ",'" << status        << "'" <<  /* 291  */
      ",'" << partyID       << "'" <<  /* 448  */
      ",'" << partyRole     << "'" <<  /* 452  */
      ",'" << accountID     << "'" <<  /* 1    */
      ",'" << accountType   << "'" <<  /* 581  */
      ",'" << currency      << "'" <<  /* 15   */
      ",'" << ratio         << "'" <<  /* 898  */
      ",'" << startCash     << "'" <<  /* 921  */
      ",'" << endCash       << "'" <<  /* 922  */
      ",'" << text          << "'" <<  /* 58   */
    ") ON DUPLICATE KEY UPDATE `ReqID` = '" << reqID << "'";
  FIX::MySQLQuery q( s.str() );
  m_sql->execute( q );

  /* Insert Collateral Responce - balance */
  std::ostringstream s2;
  s2 << "INSERT INTO `balance` " <<
    "( " <<
      "  `AccountID`      " <<  /* 1    */
      " ,`Currency`       " <<  /* 15   */
      " ,`Ratio`          " <<  /* 898  */
      " ,`StartCash`      " <<  /* 921  */
      " ,`EndCash`        " <<  /* 922  */
      " ,`TransactTime`   " <<  /* 60   */
    ") VALUES ("            <<
      " '" << accountID     << "'" <<  /* 1    */
      ",'" << currency      << "'" <<  /* 15   */
      ",'" << ratio         << "'" <<  /* 898  */
      ",'" << startCash     << "'" <<  /* 921  */
      ",'" << endCash       << "'" <<  /* 922  */
      ",'" << transactTime  << "'" <<  /* 60   */
    ") ON DUPLICATE KEY UPDATE `AccountID` = '" << accountID << "'";
  FIX::MySQLQuery q2( s2.str() );
  m_sql->execute( q2 );

}
