
#ifndef SerTana_NET_TIMER_H
#define SerTana_NET_TIMER_H

#include <boost/noncopyable.hpp>

#include <SerTana/Tools/Atomic.h>
#include <SerTana/Tools/TimestampTool.h>
#include <SerTana/net/Callbacks.h>

namespace SerTana
{
namespace net
{
///
/// Internal class for timer event.
///
class Timer : boost::noncopyable
{
 public:
  Timer(const TimerCallback& cb, TimestampTool when, double interval)
    : callback_(cb),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet())
  { }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  Timer(TimerCallback&& cb, TimestampTool when, double interval)
    : callback_(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet())
  { }
#endif

  void run() const
  {
    callback_();
  }

  TimestampTool expiration() const  { return expiration_; }
  bool repeat() const { return repeat_; }
  int64_t sequence() const { return sequence_; }

  void restart(TimestampTool now);

  static int64_t numCreated() { return s_numCreated_.get(); }

 private:
  const TimerCallback callback_;
  TimestampTool expiration_;
  const double interval_;
  const bool repeat_;
  const int64_t sequence_;

  static AtomicInt64 s_numCreated_;
};
}
}
#endif  // MUDUO_NET_TIMER_H
