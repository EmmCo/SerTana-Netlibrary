#ifndef COUNTDOWNLATCHTOOL_H
#define COUNTDOWNLATCHTOOL_H




#include <SerTana/Tools/ConditionTool.h>
#include <SerTana/Tools/Mutex.h>

#include <boost/noncopyable.hpp>

namespace SerTana
{

class CountDownLatchTool : boost::noncopyable
{
 public:

  explicit CountDownLatchTool(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  ConditionTool condition_;
  int count_;
};

}







#endif // COUNTDOWNLATCHTOOL_H
