// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//


#include <SerTana/Tools/CountDownLatchTool.h>

using namespace SerTana;

CountDownLatchTool::CountDownLatchTool(int count)
  : mutex_(),
    condition_(mutex_),
    count_(count)
{
}

void CountDownLatchTool::wait()
{
  MutexLockGuard lock(mutex_);
  while (count_ > 0)
  {
    condition_.wait();
  }
}

void CountDownLatchTool::countDown()
{
  MutexLockGuard lock(mutex_);
  --count_;
  if (count_ == 0)
  {
    condition_.notifyAll();
  }
}

int CountDownLatchTool::getCount() const
{
  MutexLockGuard lock(mutex_);
  return count_;
}

