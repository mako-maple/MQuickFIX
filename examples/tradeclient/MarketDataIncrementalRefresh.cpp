#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* X  */
void Application::onMessage(const FIX44::MarketDataIncrementalRefresh& message, const FIX::SessionID& sessionID )
{
//  std::cout << std::endl << "<X> MarketDataIncrementalRefresh:" << message.toXML() << std::endl;

  /* init message data */
  /* 52   */ FIX::FieldBase respDateTime(FIX::FIELD::SendingTime, "");
  /* 262  */ FIX::FieldBase reqID(FIX::FIELD::MDReqID, "NA");

  /* get message data */
  /* 52  */ message.getHeader().getFieldIfSet(respDateTime);
  /* 262 */ message.getFieldIfSet(reqID);

  /* Update RespDateTime */
  std::ostringstream u;
  u << "UPDATE `market_data_request` SET " <<
    "          `RespDateTime` = '"  << respDateTime << "' " <<
    "    WHERE `ReqID` = '" << reqID << "' ";
  FIX::MySQLQuery q( u.str() );
  m_sql->execute( q );

  /* Get Request */
  std::ostringstream s;
  s << "SELECT `Depth` " <<
    "        , `Aggregate` " <<
    "     FROM `market_data_request` " <<
    "    WHERE `ReqID` = '" << reqID << "' ";
  FIX::MySQLQuery q2( s.str() );
  m_sql->execute( q2 );

  /* depth = TOP */
  if (std::string(q2.getValue(0,0)) == "1")
    InsertMarketData( message );
}

void Application::InsertMarketData( const FIX::Message& message )
{
  /* 52   */ FIX::FieldBase respDateTime(FIX::FIELD::SendingTime, "");
  /* 268  */ FIX::FieldBase entries(FIX::FIELD::NoMDEntries, "0");

  /* 52  */ message.getHeader().getFieldIfSet(respDateTime);
  /* 268 */ message.getFieldIfSet(entries);

/*
  +  RespDateTime        symbol   Bid      size     order  spred   Offer    size    order  High     Low    
  +  mm/dd hh:mm:ss.sss  USD/JPY  108.050  2000000  1      0.5000  108.055  100000  1      108.328  107.880
*/

  /* Loop: Instrment Group */
  for( int i=1; i <= std::stoi(entries.getString()); i++ )
  {
    /* 146  */ FIX44::MarketDataIncrementalRefresh::NoMDEntries e;
    message.getGroup(i, e);

    /* init group data */
    /* 55   */ FIX::FieldBase symbol(FIX::FIELD::Symbol, "NA");
    /* 269  */ FIX::FieldBase type(FIX::FIELD::MDEntryType, "0");
    /* 279  */ FIX::FieldBase action(FIX::FIELD::MDUpdateAction, "0");
    /* 270  */ FIX::FieldBase px(FIX::FIELD::MDEntryPx, "0");
    /* 271  */ FIX::FieldBase size(FIX::FIELD::MDEntrySize, "0");
    /* 346  */ FIX::FieldBase orders(FIX::FIELD::NumberOfOrders, "0");

    /* get group data */
    e.getFieldIfSet(symbol);
    e.getFieldIfSet(type);
    e.getFieldIfSet(action);
    e.getFieldIfSet(px);
    e.getFieldIfSet(size);
    e.getFieldIfSet(orders);

    /* check update */
    if ( *action.getString().c_str() != FIX::MDUpdateAction_NEW )
    {
      rate[ m_symbol[symbol.getString()] ].feedStatus = false;
      return;
    }

    std::ostringstream s;
    s <<  "INSERT INTO `market_data` SET " <<
      /*-55   */  "   `Symbol`        = '" << symbol        << "' " <<
      /*-269  */  " , `Type`          = '" << type          << "' " <<
      /*-279  */  " , `Action`        = '" << action        << "' " <<
      /*-270  */  " , `Px`            = '" << px            << "' " <<
      /*-271  */  " , `Size`          = '" << size          << "' " <<
      /*-346  */  " , `Orders`        = '" << orders        << "' " <<
      /*-52   */  " , `RespDateTime`  = '" << respDateTime  << "' " <<
    " ON DUPLICATE KEY UPDATE " <<
      "  `Symbol`       =  VALUES(`Symbol`) "      <<
      " ,`Type`         =  VALUES(`Type`) "        <<
      " ,`Action`       =  VALUES(`Action`) "      <<
      " ,`Px`           =  VALUES(`Px`) "          <<
      " ,`Size`         =  VALUES(`Size`) "        <<
      " ,`Orders`       =  VALUES(`Orders`) "      <<
      " ,`RespDateTime` =  VALUES(`RespDateTime`)";

    FIX::MySQLQuery q( s.str() );
    m_sql->execute( q );

    /* rate */
    rate[ m_symbol[symbol.getString()] ].Time = respDateTime.getString();
    rate[ m_symbol[symbol.getString()] ].feedStatus = true;
    switch( *type.getString().c_str() )
    {
      case FIX::MDEntryType_BID :
        rate[ m_symbol[symbol.getString()] ].Bid = std::stod(px.getString());
        rate[ m_symbol[symbol.getString()] ].BidSize = std::stol(size.getString());
        rate[ m_symbol[symbol.getString()] ].BidOrder = std::stol(orders.getString());
        break;

      case FIX::MDEntryType_OFFER :
        rate[ m_symbol[symbol.getString()] ].Ask = std::stod(px.getString());
        rate[ m_symbol[symbol.getString()] ].AskSize = std::stol(size.getString());
        rate[ m_symbol[symbol.getString()] ].AskOrder = std::stol(orders.getString());
        break;

      case FIX::MDEntryType_TRADING_SESSION_HIGH_PRICE :
        rate[ m_symbol[symbol.getString()] ].High = std::stod(px.getString());
        break;

      case FIX::MDEntryType_TRADING_SESSION_LOW_PRICE :
        rate[ m_symbol[symbol.getString()] ].Low = std::stod(px.getString());
        break;

      case FIX::MDEntryType_TRADING_SESSION_VWAP_PRICE :
        rate[ m_symbol[symbol.getString()] ].Spred = std::stod(px.getString());
        break;
    }
  }
}

/* - message sample Depth = TOP ---
  <body>
    <field number="262"><![CDATA[USD/JPY]]></field>
    <field number="268"><![CDATA[5]]></field>
    <group>
      <field number="279"><![CDATA[0]]></field>
      <field number="269"><![CDATA[0]]></field>                 -Bid
      <field number="278"><![CDATA[1]]></field>
      <field number="55"><![CDATA[USD/JPY]]></field>
      <field number="270"><![CDATA[108.05]]></field>
      <field number="271"><![CDATA[2000000]]></field>
      <field number="346"><![CDATA[1]]></field>
    </group>
    <group>
      <field number="279"><![CDATA[0]]></field>
      <field number="269"><![CDATA[1]]></field>                 -Offer
      <field number="278"><![CDATA[2]]></field>
      <field number="55"><![CDATA[USD/JPY]]></field>
      <field number="270"><![CDATA[108.055]]></field>
      <field number="271"><![CDATA[1000000]]></field>
      <field number="346"><![CDATA[1]]></field>
    </group>
    <group>
      <field number="279"><![CDATA[0]]></field>
      <field number="269"><![CDATA[7]]></field>                 -High
      <field number="278"><![CDATA[3]]></field>
      <field number="55"><![CDATA[USD/JPY]]></field>
      <field number="270"><![CDATA[108.328]]></field>
    </group>
    <group>
      <field number="279"><![CDATA[0]]></field>
      <field number="269"><![CDATA[8]]></field>                 -Low
      <field number="278"><![CDATA[4]]></field>
      <field number="55"><![CDATA[USD/JPY]]></field>
      <field number="270"><![CDATA[107.88]]></field>
    </group>
    <group>
      <field number="279"><![CDATA[0]]></field>
      <field number="269"><![CDATA[9]]></field>                 -Spread
      <field number="278"><![CDATA[5]]></field>
      <field number="55"><![CDATA[USD/JPY]]></field>
      <field number="270"><![CDATA[0.5000]]></field>
    </group>
  </body>
*/
