#ifndef SerTanaCONDITIONTOOL_H
#define SerTanaCONDITIONTOOL_H




#include <SerTana/Tools/Mutex.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace SerTana
{

class ConditionTool : boost::noncopyable
{
 public:
  explicit ConditionTool(MutexLock& mutex)
    : mutex_(mutex)
  {
    MCHECK(pthread_cond_init(&pcond_, NULL));
  }

  ~ConditionTool()
  {
    MCHECK(pthread_cond_destroy(&pcond_));
  }

  void wait()
  {
    MutexLock::UnassignGuard ug(mutex_);
    MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
  }

  // returns true if time out, false otherwise.
  bool waitForSeconds(double seconds);

  void notify()
  {
    MCHECK(pthread_cond_signal(&pcond_));
  }

  void notifyAll()
  {
    MCHECK(pthread_cond_broadcast(&pcond_));
  }

 private:
  MutexLock& mutex_;
  pthread_cond_t pcond_;
};

}







#endif // SerTanaCONDITIONTOOL_H
