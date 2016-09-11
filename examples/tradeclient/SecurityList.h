#include "quickfix/Message.h"

namespace FIX44
{

  class AttrGrp : public Message
  {
  public:
    class NoInstrAttrib: public FIX::Group
    {
    public:
    NoInstrAttrib() : FIX::Group(870,871,FIX::message_order(871,872,0)) {}
      FIELD_SET(*this, FIX::InstrAttribType);
      FIELD_SET(*this, FIX::InstrAttribValue);
    };
  };
}
