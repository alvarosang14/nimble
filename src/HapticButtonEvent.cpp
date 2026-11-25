#include "HapticButtonEvent.h"

const QEvent::Type HapticButtonEvent::EVENT_TYPE =
  static_cast<QEvent::Type>(2000);

HapticButtonEvent::HapticButtonEvent()
  :QEvent(EVENT_TYPE)
{
}

HapticButtonEvent::~HapticButtonEvent()
{
}
