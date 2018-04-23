
#ifndef SerTana_NET_TIMERID_H
#define SerTana_NET_TIMERID_H

#include <SerTana/Tools/copyable.h>

namespace SerTana
{
namespace net
{

class Timer;

///
/// An opaque identifier, for canceling Timer.
///
class TimerId : public SerTana::copyable
{
 public:
  TimerId()
    : timer_(NULL),
      sequence_(0)
  {
  }

  TimerId(Timer* timer, int64_t seq)
    : timer_(timer),
      sequence_(seq)
  {
  }

  // default copy-ctor, dtor and assignment are okay

  friend class TimerQueue;

 private:
  Timer* timer_;
  int64_t sequence_;
};

}
}

#endif  // MUDUO_NET_TIMERID_H
