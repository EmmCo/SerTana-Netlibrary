

#ifndef SerTana_NET_EVENTLOOPTHREAD_H
#define SerTana_NET_EVENTLOOPTHREAD_H

#include <SerTana/Tools/ConditionTool.h>
#include <SerTana/Tools/Mutex.h>
#include <SerTana/Tools/ThreadTool.h>

#include <boost/noncopyable.hpp>

namespace SerTana
{
namespace net
{

class EventLoop;

class EventLoopThread : boost::noncopyable
{
 public:
  typedef boost::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                  const string& name = string());
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();

  EventLoop* loop_;
  bool exiting_;
  ThreadTool thread_;
  MutexLock mutex_;
  ConditionTool cond_;
  ThreadInitCallback callback_;
};

}
}

#endif  // SerTana_NET_EVENTLOOPTHREAD_H

