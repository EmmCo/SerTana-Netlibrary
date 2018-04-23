#ifndef SERTANA_P2PSERVER_H
#define SERTANA_P2PSERVER_H



#include <SerTana/net/TcpServer.h>
#include <boost/noncopyable.hpp>
#include <list>
#include <SerTana/net/P2P/P2PTools.h>

namespace SerTana
{
namespace net
{

class P2PServer : boost::noncopyable
{
 public:
  typedef boost::function<void (
            //const HttpRequest&,HttpResponse*
            )> HttpCallback;


  typedef  std::list<p2p_msg_device_info_t >  Devlisttype;

  P2PServer(EventLoop* loop,
             const InetAddress& listenAddr,
             const string& name,
             TcpServer::Option option = TcpServer::kNoReusePort);

  ~P2PServer(){}  // force out-line dtor, for scoped_ptr members.

  EventLoop* getLoop() const { return server_.getLoop(); }

  /// Not thread safe, callback be registered before calling start().
  void setHttpCallback(const HttpCallback& cb)
  {
    httpCallback_ = cb;
     // std::map<string, TcpConnectionPtr> connections_
  }

  void setThreadNum(int numThreads)
  {
    server_.setThreadNum(numThreads);
  }
  void test();
  void start();



 private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 TimestampTool receiveTime);

  void DeviceListInsert_unique(const struct p2p_msg_device_info_t &pdevice_info);
  void Process_Ping_Cmd(const TcpConnectionPtr& conn, const struct p2p_msg_ping_t *pping, const struct sockaddr_in *pclient_addr);
  void Send_Punch_Hole_Cmd_to_Peer(const TcpConnectionPtr& conn, const struct p2p_msg_device_info_t *psrc_peer);
  void Process_Query_Device_Info_Cmd(const TcpConnectionPtr& conn, uint64_t src_devcie_sn, uint64_t query_device_sn);

  //void onRequest(const TcpConnectionPtr&, const HttpRequest&);





  TcpServer server_;
  HttpCallback httpCallback_;
  Devlisttype Devlist;
};

}
}



#endif // SERTANA_P2PSERVER_H
