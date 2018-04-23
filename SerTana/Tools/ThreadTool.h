#ifndef THREADTOOL_H
#define THREADTOOL_H


#include <SerTana/Tools/Atomic.h>
#include <SerTana/Tools/CountDownLatchTool.h>
#include <SerTana/Tools/Types.h>


#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

namespace SerTana
{

class ThreadTool : boost::noncopyable
{
 public:
  typedef boost::function<void ()> ThreadFunc;

  explicit ThreadTool(const ThreadFunc&, const string& name = string());
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  explicit ThreadTool(ThreadFunc&&, const string& name = string());
#endif
  ~ThreadTool();

  void start();
  int join(); // return pthread_join()

  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return tid_; }
  const string& name() const { return name_; }

  static int numCreated() { return numCreated_.get(); }

 private:
  void setDefaultName();

  bool       started_;
  bool       joined_;
  pthread_t  pthreadId_;
  pid_t      tid_;
  ThreadFunc func_;
  string     name_;
  CountDownLatchTool latch_;

  static AtomicInt32 numCreated_;
};

}



#endif // THREADTOOL_H
