#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>
#include "SecurityList.h"

/* y  */
void Application::onMessage(const FIX44::SecurityList& message, const FIX::SessionID& sessionID )
{
//  std::cout << std::endl << "<y> SecurityList:" << message.toXML() << std::endl;

  /* INIT Message Data */
  /* 52   */ FIX::FieldBase respDateTime( FIX::FIELD::SendingTime, "" );
  /* 320  */ FIX::FieldBase reqID( FIX::FIELD::SecurityReqID, "0" );
  /* 322  */ FIX::FieldBase respID( FIX::FIELD::SecurityResponseID, "0" );
  /* 560  */ FIX::FieldBase result( FIX::FIELD::SecurityRequestResult, "0" );
  /* 146  */ FIX::FieldBase symbolCount( FIX::FIELD::NoRelatedSym, "0" );
  /* 55   */ FIX::FieldBase symbol(FIX::FIELD::Symbol, "NA");

  /* Get Message Data */
  /* 52   */ message.getHeader().getFieldIfSet( respDateTime );
  /* 320  */ message.getFieldIfSet( reqID );
  /* 322  */ message.getFieldIfSet( respID );
  /* 560  */ message.getFieldIfSet( result );
  /* 146  */ message.getFieldIfSet( symbolCount );

  /* Insert Security List */
  std::ostringstream s1;
  s1 << "INSERT INTO `security_list_request` " <<
    "( " <<
    "  `ReqID`, " <<
    "  `RespDateTime`, " <<
    "  `RespID`, " <<
    "  `Result`, " <<
    "  `SymbolCount` " <<
    ") VALUES ( " <<
    "  '" << reqID << "'," <<
    "  '" << respDateTime << "'," <<
    "  '" << respID << "'," <<
    "  '" << result << "'," <<
    "  '" << symbolCount << "'" <<
    ") ON DUPLICATE KEY UPDATE " <<
    "  `RespDateTime` = VALUES(`RespDateTime` ), " <<
    "  `RespID`       = VALUES(`RespID` ), " <<
    "  `Result`       = VALUES(`Result` ), " <<
    "  `SymbolCount`  = VALUES(`SymbolCount` ) ";
  FIX::MySQLQuery q1( s1.str() );
  m_sql->execute( q1 );

  /* Insert security list :: Currency Pair Symbol - Decimal Places  */
  char sep = ' ';
  std::ostringstream s2;
  s2 <<  "INSERT INTO `security_list` " <<
    "( " <<
    "  `Symbol`, " <<
    "  `DecPlaces_Str`, " <<
    "  `DecPlaces_Forex`, " <<
    "  `ReqID` " <<
    ") VALUES ";

  /* init rate vector */
  rate.clear();
  rate.shrink_to_fit();
  rate.resize(std::stoi(symbolCount.getString()));

  /* Loop: Currency Pair */
  for( int i=1; i <= std::stoi(symbolCount.getString()); i++ )
  {
    /* 146  */ FIX44::SecurityList::NoRelatedSym r;
               message.getGroup(i, r);
    /* 55   */ // FIX::FieldBase symbol(FIX::FIELD::Symbol, "NA");
    /* 870  */ FIX::FieldBase attrCount(FIX::FIELD::NoInstrAttrib, "0");
    /* 872  */ FIX::FieldBase value_str(FIX::FIELD::InstrAttribValue, "0");
    /* 872  */ FIX::FieldBase value_forex(FIX::FIELD::InstrAttribValue, "0");

    /* 55   */ r.getFieldIfSet(symbol);
    /* 870  */ r.getFieldIfSet(attrCount);

    /* Loop: Attribute - Decimal Places Type */
    for( int j=1; j <= std::stoi(attrCount.getString()); j++ )
    {
      /* 870  */ FIX44::AttrGrp::NoInstrAttrib g;
      r.getGroup(j, g);
      /* 871  */ FIX::FieldBase type(FIX::FIELD::InstrAttribType, "0");
      g.getFieldIfSet(type);
      if( type.getString() == "16" ) g.getFieldIfSet(value_str);
      if( type.getString() == "18" ) g.getFieldIfSet(value_forex);
    }

    /* Insert security list :: Currency Pair Symbol - Decimal Places  */
    s2 << sep << " (" <<
      " '" << symbol << "'" <<
      ",'" << value_str << "'" <<
      ",'" << value_forex << "'" <<
      ",'" << reqID << "'" <<
    ") ";
    sep = ',';

    /* Symbol : map - vector */
    m_symbol[symbol.getString()] = (i - 1);
    rate[ m_symbol[symbol.getString()] ].Symbol = symbol.getString();
  }

  /* Insert security list :: Currency Pair Symbol - Decimal Places  */
  s2 << " ON DUPLICATE KEY UPDATE " <<
    "  `DecPlaces_Str`   = VALUES(`DecPlaces_Str`),   " <<
    "  `DecPlaces_Forex` = VALUES(`DecPlaces_Forex`), " <<
    "  `ReqID`           = VALUES(`ReqID`)            ";
  FIX::MySQLQuery q2( s2.str() );
  m_sql->execute( q2 );

  /* Get Market Data */
  MarketDataRequest( symbol.getString(), 1, true, true );
}
