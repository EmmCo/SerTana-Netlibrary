

#ifndef SerTana_NET_POLLER_POLLPOLLER_H
#define SerTana_NET_POLLER_POLLPOLLER_H

#include <SerTana/net/Poller.h>

#include <vector>

struct pollfd;

namespace SerTana
{
namespace net
{

///
/// IO Multiplexing with poll(2).
///
class PollPoller : public Poller
{
 public:

  PollPoller(EventLoop* loop);
  virtual ~PollPoller();

  virtual TimestampTool poll(int timeoutMs, ChannelList* activeChannels);
  virtual void updateChannel(Channel* channel);
  virtual void removeChannel(Channel* channel);

 private:
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;
  PollFdList pollfds_;
};

}
}
#endif  // SerTana_NET_POLLER_POLLPOLLER_H
