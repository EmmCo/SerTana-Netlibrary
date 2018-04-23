#ifndef SerTana_BLOCKINGQUEUE_H
#define SerTana_BLOCKINGQUEUE_H



#include <SerTana/Tools/ConditionTool.h>
#include <SerTana/Tools/Mutex.h>

#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <assert.h>

#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace SerTana
{

template<typename T>
class BlockingQueue : boost::noncopyable
{
 public:
  BlockingQueue()
    : mutex_(),
      notEmpty_(mutex_),
      queue_()
  {
  }

  void put(const T& x)
  {
    MutexLockGuard lock(mutex_);
    queue_.push_back(x);
    notEmpty_.notify(); // wait morphing saves us
    // http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  void put(T&& x)
  {
    MutexLockGuard lock(mutex_);
    queue_.push_back(std::move(x));
    notEmpty_.notify();
  }
  // FIXME: emplace()
#endif

  T take()
  {
    MutexLockGuard lock(mutex_);
    // always use a while-loop, due to spurious wakeup
    while (queue_.empty())
    {
      notEmpty_.wait();
    }
    assert(!queue_.empty());
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    T front(std::move(queue_.front()));
#else
    T front(queue_.front());
#endif
    queue_.pop_front();
    return front;
  }

  size_t size() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable MutexLock mutex_;
  ConditionTool     notEmpty_;
  std::deque<T>     queue_;
};

}




#endif // SerTana_BLOCKINGQUEUE_H
