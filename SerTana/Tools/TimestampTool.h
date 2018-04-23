
#ifndef Wsadasdasdasdasfczxc_H
#define Wsadasdasdasdasfczxc_H


#include <SerTana/Tools/copyable.h>
#include <SerTana/Tools/Types.h>
#include <boost/operators.hpp>

namespace SerTana
{

class TimestampTool: public SerTana::copyable
                      ,public boost::equality_comparable<TimestampTool>
                      ,public boost::less_than_comparable<TimestampTool>
{
 public:
  ///
  /// Constucts an invalid Timestamp.
  ///
  TimestampTool()
    : microSecondsSinceEpoch_(0)
  {
  }

  ///
  /// Constucts a Timestamp at specific time
  ///
  /// @param microSecondsSinceEpoch
  explicit TimestampTool(int64_t microSecondsSinceEpochArg)
    : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
  {
  }

  void swap(TimestampTool& that)
  {
    std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
  }

  // default copy/assignment/dtor are Okay

  string toString() const;
  string toFormattedString(bool showMicroseconds = true) const;

  bool valid() const { return microSecondsSinceEpoch_ > 0; }

  // for internal usage.
  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  time_t secondsSinceEpoch() const
  { return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }

  ///
  /// Get time of now.
  ///
  static TimestampTool now();
  static TimestampTool invalid()
  {
    return TimestampTool();
  }

  static TimestampTool fromUnixTime(time_t t)
  {
    return fromUnixTime(t, 0);
  }

  static TimestampTool fromUnixTime(time_t t, int microseconds)
  {
    return TimestampTool(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
  }

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(TimestampTool lhs, TimestampTool rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(TimestampTool lhs, TimestampTool rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

///
/// Gets time difference of two timestamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microsecond
/// resolution for next 100 years.
inline double timeDifference(TimestampTool high, TimestampTool low)
{
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / TimestampTool::kMicroSecondsPerSecond;
}

///
/// Add @c seconds to given timestamp.
///
/// @return timestamp+seconds as Timestamp
///
inline TimestampTool addTime(TimestampTool timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * TimestampTool::kMicroSecondsPerSecond);
  return TimestampTool(timestamp.microSecondsSinceEpoch() + delta);
}

}


 #endif // SerTanaTIMESTAMPTOOL_H
