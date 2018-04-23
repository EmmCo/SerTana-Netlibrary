
#include <SerTana/net/Timer.h>

using namespace SerTana;
using namespace SerTana::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(TimestampTool now)
{
  if (repeat_)
  {
    expiration_ = addTime(now, interval_);
  }
  else
  {
    expiration_ = TimestampTool::invalid();
  }
}
