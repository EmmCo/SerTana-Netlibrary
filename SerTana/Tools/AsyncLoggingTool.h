#ifndef SerTana_ASYNCLOGGINGTOOL_H
#define SerTana_ASYNCLOGGINGTOOL_H



#include <SerTana/Tools/BlockingQueue.h>
#include <SerTana/Tools/BoundedBlockingQueue.h>
#include <SerTana/Tools/CountDownLatchTool.h>
#include <SerTana/Tools/Mutex.h>
#include <SerTana/Tools/ThreadTool.h>
#include <SerTana/Tools/LogStreamTool.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace SerTana
{

class AsyncLoggingTool : boost::noncopyable
{
 public:

  AsyncLoggingTool(const string& basename,
               off_t rollSize,
               int flushInterval = 3);

  ~AsyncLoggingTool()
  {
    if (running_)
    {
      stop();
    }
  }

  void append(const char* logline, int len);

  void start()
  {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop()
  {
    running_ = false;
    cond_.notify();
    thread_.join();
  }

 private:

  // declare but not define, prevent compiler-synthesized functions
  AsyncLoggingTool(const AsyncLoggingTool&);  // ptr_container
  void operator=(const AsyncLoggingTool&);  // ptr_container

  void threadFunc();

  typedef SerTana::detail::FixedBuffer<SerTana::detail::kLargeBuffer> Buffer;
  typedef boost::ptr_vector<Buffer> BufferVector;
  typedef BufferVector::auto_type BufferPtr;

  const int flushInterval_;
  bool running_;
  string basename_;
  off_t rollSize_;
  SerTana::ThreadTool thread_;
  SerTana::CountDownLatchTool latch_;
  SerTana::MutexLock mutex_;
  SerTana::ConditionTool cond_;
  BufferPtr currentBuffer_;
  BufferPtr nextBuffer_;
  BufferVector buffers_;
};

}





#endif // SerTana_ASYNCLOGGINGTOOL_H
