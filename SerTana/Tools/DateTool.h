#ifndef DATETOOL_H
#define DATETOOL_H





#include <SerTana/Tools/copyable.h>
#include <SerTana/Tools/Types.h>

struct tm;

namespace SerTana
{

///
/// Date in Gregorian calendar.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///
class DateTool : public SerTana::copyable
          // public boost::less_than_comparable<Date>,
          // public boost::equality_comparable<Date>
{
 public:

  struct YearMonthDay
  {
    int year; // [1900..2500]
    int month;  // [1..12]
    int day;  // [1..31]
  };

  static const int kDaysPerWeek = 7;
  static const int kJulianDayOf1970_01_01;

  ///
  /// Constucts an invalid Date.
  ///
  DateTool()
    : julianDayNumber_(0)
  {}

  ///
  /// Constucts a yyyy-mm-dd Date.
  ///
  /// 1 <= month <= 12
  DateTool(int year, int month, int day);

  ///
  /// Constucts a Date from Julian Day Number.
  ///
  explicit DateTool(int julianDayNum)
    : julianDayNumber_(julianDayNum)
  {}

  ///
  /// Constucts a Date from struct tm
  ///
  explicit DateTool(const struct tm&);

  // default copy/assignment/dtor are Okay

  void swap(DateTool& that)
  {
    std::swap(julianDayNumber_, that.julianDayNumber_);
  }

  bool valid() const { return julianDayNumber_ > 0; }

  ///
  /// Converts to yyyy-mm-dd format.
  ///
  string toIsoString() const;

  struct YearMonthDay yearMonthDay() const;

  int year() const
  {
    return yearMonthDay().year;
  }

  int month() const
  {
    return yearMonthDay().month;
  }

  int day() const
  {
    return yearMonthDay().day;
  }

  // [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
  int weekDay() const
  {
    return (julianDayNumber_+1) % kDaysPerWeek;
  }

  int julianDayNumber() const { return julianDayNumber_; }

 private:
  int julianDayNumber_;
};

inline bool operator<(DateTool x, DateTool y)
{
  return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(DateTool x, DateTool y)
{
  return x.julianDayNumber() == y.julianDayNumber();
}

}




#endif // DATETOOL_H
