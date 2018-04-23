#ifndef SERTANA_P2PCLIENT_H
#define SERTANA_P2PCLIENT_H


#include <SerTana/net/TcpServer.h>
#include <SerTana/net/TcpClient.h>
#include <boost/noncopyable.hpp>
#include <list>
#include <SerTana/net/P2P/P2PTools.h>
#include <SerTana/net/Callbacks.h>
namespace SerTana
{
namespace net
{

class P2PClient : boost::noncopyable
{


 public:

                P2PClient(EventLoop*      loop    ,
                         InetAddress & LocalAddr ,
                         InetAddress & ServerAddr,
                         uint64_t      _device_sn,
                         const string& servename,
                         const string& clietname
                          );

  ~P2PClient(){}  // force out-line dtor, for scoped_ptr members.

  EventLoop* getServeLoop()  const { return server_->getLoop(); }
  void  Punch_Cmd(uint64_t test_peer_device_sn);//Send Punch Hole cmd to Server


  void setThreadNum(int numThreads)
  {
    server_->setThreadNum(numThreads);
  }

  void listen()
  {
      server_->start();
  }
  void connect()
  {
      serclient_->connect();
  }

 private:
  EventLoop*   loop_;

  TcpServerPtr server_;
  TcpClientPtr serclient_;
  TcpClientPtr peerclient_;
  uint64_t     device_sn;

  void onMessageServer(const TcpConnectionPtr& conn,
                       Buffer* buf,
                       TimestampTool receiveTime);

  void onMessagePeer(const TcpConnectionPtr &conn,
                     Buffer *buf,
                     TimestampTool receiveTime);


  void onServeConnection(const TcpConnectionPtr& conn);
  void onPeerConnection(const TcpConnectionPtr& conn);

  void Connect_Peer(struct p2p_msg_device_info_t *pdevice_info);

};

}
}








#endif // P2PCLIENT_H
