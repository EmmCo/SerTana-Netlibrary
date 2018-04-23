#ifndef SerTana_THREADPOOLTOOL_H
#define SerTana_THREADPOOLTOOL_H


#include <SerTana/Tools/ConditionTool.h>
#include <SerTana/Tools/Mutex.h>
#include <SerTana/Tools/ThreadTool.h>
#include <SerTana/Tools/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace SerTana
{

class ThreadPoolTool : boost::noncopyable
{
 public:
  typedef boost::function<void ()> Task;

  explicit ThreadPoolTool(const string& nameArg = string("ThreadPool"));
  ~ThreadPoolTool();

  // Must be called before start().
  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
  void setThreadInitCallback(const Task& cb)
  { threadInitCallback_ = cb; }

  void start(int numThreads);
  void stop();

  const string& name() const
  { return name_; }

  size_t queueSize() const;

  // Could block if maxQueueSize > 0
  void run(const Task& f);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  void run(Task&& f);
#endif

 private:
  bool isFull() const;
  void runInThread();
  Task take();

  mutable MutexLock mutex_;
  ConditionTool notEmpty_;
  ConditionTool notFull_;
  string name_;
  Task threadInitCallback_;
  boost::ptr_vector<SerTana::ThreadTool> threads_;
  std::deque<Task> queue_;
  size_t maxQueueSize_;
  bool running_;
};

}


#endif // SerTana_THREADPOOLTOOL_H
