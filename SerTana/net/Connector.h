#ifndef SerTana_NET_CONNECTOR_H
#define SerTana_NET_CONNECTOR_H

#include <SerTana/net/InetAddress.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <SerTana/net/Socket.h>

namespace SerTana
{
namespace net
{

class Channel;
class EventLoop;

class Connector : boost::noncopyable,
                  public boost::enable_shared_from_this<Connector>
{
 public:
  typedef boost::function<void (int sockfd)> NewConnectionCallback;

  Connector(EventLoop* loop, const InetAddress& serverAddr);
  Connector(EventLoop* loop, const InetAddress& serverAddr,const InetAddress& localAddr);


  ~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  void start();  // can be called in any thread
  void restart();  // must be called in loop thread
  void stop();  // can be called in any thread

  const InetAddress& serverAddress() const { return serverAddr_; }

 private:
  enum States { kDisconnected, kConnecting, kConnected };
  static const int kMaxRetryDelayMs = 30*1000;
  static const int kInitRetryDelayMs = 500;

  void setState(States s) { state_ = s; }
  void startInLoop();
  void stopInLoop();
  void connect();
  void connecting(int sockfd);
  void handleWrite();
  void handleError();
  void retry(int sockfd);
  int removeAndResetChannel();
  void resetChannel();

  EventLoop* loop_;
  InetAddress serverAddr_;
  InetAddress localAddr_ ;
  Socket      ConnectSocket_;

  bool connect_; // atomic
  States state_;  // FIXME: use atomic variable
  boost::scoped_ptr<Channel> channel_;
  NewConnectionCallback newConnectionCallback_;
  int retryDelayMs_;
};

}
}

#endif  // MUDUO_NET_CONNECTOR_H
