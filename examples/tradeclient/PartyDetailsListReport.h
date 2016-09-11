#include "quickfix/Message.h"

namespace FIX
{
  namespace FIELD
  {
    const int PartyDetailsListReportID = 1510;
    const int PartyDetailsListRequestID = 1505;
    const int PartyDetailsRequestResult = 1511;
    const int PartyDetailsType = 7593;
    const int NoPartyList = 1513;
  }
}

namespace FIX
{
  DEFINE_STRING(PartyDetailsListReportID);
  DEFINE_STRING(PartyDetailsListRequestID);
  DEFINE_INT(PartyDetailsRequestResult);
  DEFINE_INT(PartyDetailsType);
  DEFINE_NUMINGROUP(NoPartyList);
}

namespace FIX44
{

  class PartyDetailsListReport : public Message
  {
  public:
    PartyDetailsListReport() : Message(MsgType()) {}
    PartyDetailsListReport(const FIX::Message& m) : Message(m) {}
    PartyDetailsListReport(const Message& m) : Message(m) {}
    PartyDetailsListReport(const PartyDetailsListReport& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("CG"); }

    PartyDetailsListReport(
      const FIX::PartyDetailsListReportID& aPartyDetailsListReportID ,
      const FIX::PartyDetailsListRequestID& aPartyDetailsListRequestID ,
      const FIX::PartyDetailsRequestResult& aPartyDetailsRequestResult)
    : Message(MsgType())
    {
      set(aPartyDetailsListReportID);
      set(aPartyDetailsListRequestID);
      set(aPartyDetailsRequestResult);
    }

    FIELD_SET(*this, FIX::PartyDetailsListReportID);
    FIELD_SET(*this, FIX::PartyDetailsListRequestID);
    FIELD_SET(*this, FIX::PartyDetailsRequestResult);
    FIELD_SET(*this, FIX::TotNumReports);
    FIELD_SET(*this, FIX::PartyDetailsType);
    FIELD_SET(*this, FIX::TransactTime);
    FIELD_SET(*this, FIX::Text);
    FIELD_SET(*this, FIX::NoPartyList);
    class NoPartyList: public FIX::Group
    {
    public:
    NoPartyList() : FIX::Group(1513,448,FIX::message_order(448,447,452,0)) {}
      FIELD_SET(*this, FIX::PartyID);
      FIELD_SET(*this, FIX::PartyIDSource);
      FIELD_SET(*this, FIX::PartyRole);
    };
  };

}
