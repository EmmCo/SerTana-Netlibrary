
#include <SerTana/net/Poller.h>
#include <SerTana/net/poller/PollPoller.h>
#include <SerTana/net/poller/EPollPoller.h>

#include <stdlib.h>

using namespace SerTana::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  if (::getenv("SERTANA_USE_POLL"))
  {
    return new PollPoller(loop);
  }
  else
  {
    return new EPollPoller(loop);
  }
}
