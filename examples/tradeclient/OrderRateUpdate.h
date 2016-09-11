#include "quickfix/Message.h"

namespace FIX44
{

  class OrderRateUpdate : public Message
  {
  public:
    OrderRateUpdate() : Message(MsgType()) {}
    OrderRateUpdate(const FIX::Message& m) : Message(m) {}
    OrderRateUpdate(const Message& m) : Message(m) {}
    OrderRateUpdate(const OrderRateUpdate& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("PU"); }

    OrderRateUpdate(
      const FIX::OrderID& aOrderID ,
      const FIX::ClOrdID& aClOrdID ,
      const FIX::OrigClOrdID& aOrigClOrdID ,
      const FIX::OrdType& aOrdType )
    : Message(MsgType())
    {
      set(aOrderID);
      set(aClOrdID);
      set(aOrigClOrdID);
      set(aOrdType);
    }

    FIELD_SET(*this, FIX::OrderID);
    FIELD_SET(*this, FIX::ClOrdID);
    FIELD_SET(*this, FIX::OrigClOrdID);
    FIELD_SET(*this, FIX::OrdType);
    FIELD_SET(*this, FIX::StopPx);
    FIELD_SET(*this, FIX::NoPartyIDs);
    class NoPartyIDs: public FIX::Group
    {
    public:
    NoPartyIDs() : FIX::Group(453,448,FIX::message_order(448,452,0)) {}
      FIELD_SET(*this, FIX::PartyID);
      FIELD_SET(*this, FIX::PartyRole);
    };
  };

}
