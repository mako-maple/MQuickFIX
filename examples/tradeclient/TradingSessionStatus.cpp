#include "config.h"

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

/* h  */
void Application::onMessage(const FIX44::TradingSessionStatus& message, const FIX::SessionID& sessionID )
{
  const FIX::Dictionary& dic = m_settings.get( sessionID );
  if (dic.has("DEGUG_h_TradingSessionStatus"))
    std::cout << message.toXML() << std::endl;
}
