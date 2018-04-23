#include <SerTana/Tools/LoggingTool.h>
#include <boost/bind.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <SerTana/net/P2P/P2PServer.h>
#include <SerTana/net/P2P/P2PTools.h>
#include <SerTana/net/EventLoop.h>

using namespace SerTana;
using namespace SerTana::net;


void P2PServer::DeviceListInsert_unique( const struct p2p_msg_device_info_t &pdevice_info)
{
    for(Devlisttype::iterator it = Devlist.begin(); it != Devlist.end(); it++)
    {
        if(it->device_sn==pdevice_info.device_sn)
         {
            p2p_msg_device_info_t &msg_device_info =*it;
            msg_device_info.ip_addr      = pdevice_info.ip_addr;
            msg_device_info.network_type = pdevice_info.network_type;
            msg_device_info.port         = pdevice_info.port;
            return ;
         }
    }
    Devlist.push_front(pdevice_info);
}

void P2PServer::Process_Ping_Cmd(const TcpConnectionPtr& conn,const struct p2p_msg_ping_t *pping, const struct sockaddr_in *pclient_addr)
{
    assert(pping != NULL);
    assert(pclient_addr != NULL);

    struct p2p_msg_device_info_t pdevice_info;

    pdevice_info.device_sn    = pping->device_sn      ;
    pdevice_info.network_type = pping->network_type   ;
    pdevice_info.ip_addr      = pclient_addr->sin_addr;
    pdevice_info.port         = pclient_addr->sin_port;

    DeviceListInsert_unique(pdevice_info);
}


void P2PServer::Send_Punch_Hole_Cmd_to_Peer(const TcpConnectionPtr& conn, const struct p2p_msg_device_info_t *psrc_peer)
{
    assert(psrc_peer != NULL);

    struct p2p_msg_head_t  pmsg  ;
    struct p2p_msg_device_info_t *pdevice_info = NULL;
    int total_len = sizeof(struct p2p_msg_head_t) + sizeof(struct p2p_msg_device_info_t);

    pmsg.magic   = P2P_TRANSFER_MAGIC;
    pmsg.cmd     = P2P_TRANSFER_PUNCH_HOLE;
    pmsg.cmd_len = sizeof(struct p2p_msg_device_info_t);
    pdevice_info = (struct p2p_msg_device_info_t *)(pmsg.cmd_data);

    pdevice_info->device_sn    = psrc_peer->device_sn;
    pdevice_info->ip_addr      = psrc_peer->ip_addr;
    pdevice_info->port         = psrc_peer->port;
    pdevice_info->network_type = psrc_peer->network_type;

    conn->send(&pmsg, total_len);
}


void P2PServer::Process_Query_Device_Info_Cmd(const TcpConnectionPtr& conn, uint64_t src_devcie_sn, uint64_t query_device_sn)
{


    struct p2p_msg_head_t          presponse          ;
    struct p2p_msg_device_info_t   psrc_device_info   ;
    struct p2p_msg_device_info_t*  pquery_device_info ;

    int total_len = sizeof(struct p2p_msg_head_t) + sizeof(struct p2p_msg_device_info_t);

    int num = 0;

    presponse.magic    = P2P_TRANSFER_MAGIC;
    presponse.cmd      = P2P_TRANSFER_UNKNOWN_CMD;
    presponse.cmd_len  = sizeof(struct p2p_msg_device_info_t);
    pquery_device_info = (struct p2p_msg_device_info_t *)(presponse.cmd_data);

    for(Devlisttype::iterator it = Devlist.begin(); it != Devlist.end(); it++)
    {
        if(it->device_sn == query_device_sn)
        {
           presponse.cmd = P2P_TRANSFER_QUERY_DEVICE_INFO_RESPONSE;
           pquery_device_info->device_sn    = it->device_sn;
           pquery_device_info->ip_addr      = it->ip_addr;
           pquery_device_info->port         = it->port;
           pquery_device_info->network_type = it->network_type;
           num++;
        }
        if (it->device_sn == src_devcie_sn)
        {
           psrc_device_info.device_sn    = it->device_sn;
           psrc_device_info.ip_addr      = it->ip_addr;
           psrc_device_info.port         = it->port;
           psrc_device_info.network_type = it->network_type;
           num += 1;
        }
        if (num == 2)
        {
            break;
        }
    }

     Send_Punch_Hole_Cmd_to_Peer(conn, &psrc_device_info);

    // 回复请求者
    conn->send(&presponse, total_len);
}


void P2PServer::start()
{
  LOG_WARN << "P2P Hole Server[ " << server_.name()
    << " ]  starts listenning on  " << server_.ipPort();
  server_.start();
}

void P2PServer::onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    //conn->setContext(P2PContext());
  }


}

void P2PServer::onMessage(const TcpConnectionPtr& conn,//We get a message pack
                           Buffer* buf,
                           TimestampTool receiveTime)
{
    struct p2p_msg_head_t *prequest = (p2p_msg_head_t *) (buf->peek());
    buf->retrieve(sizeof(struct p2p_msg_head_t));

    struct p2p_msg_ping_t *pping = NULL;
    struct p2p_msg_device_info_t *pquery_device_info = NULL;

    switch (prequest->cmd)
    {
    case P2P_TRANSFER_PING:

        pping = (p2p_msg_ping_t *) (buf->peek());
        buf->retrieve(sizeof (struct p2p_msg_ping_t));
        Process_Ping_Cmd(conn, pping, (struct sockaddr_in *)conn->peerAddress().getSockAddr());

        break;

    case P2P_TRANSFER_QUERY_DEVICE_INFO_REQUEST:

        pquery_device_info = (p2p_msg_device_info_t *) (buf->peek());
        buf->retrieve(sizeof (struct p2p_msg_device_info_t));
        Process_Query_Device_Info_Cmd(conn, prequest->src_device_sn, pquery_device_info->device_sn);

        break;

    default:
        break;
    }
}
void P2PServer::test()
{
  EventLoop _loop;
  P2PServer _p2pserver(&_loop,InetAddress(P2P_TRANSFER_SERVER_PORT),"Hole Server");
  _p2pserver.setThreadNum(5);
  _p2pserver.start();
  _loop.loop();
}


P2PServer::P2PServer(EventLoop* loop,
           const InetAddress& listenAddr,
           const string& name,
           TcpServer::Option option):server_(loop,listenAddr,name,option)
{
    server_.setConnectionCallback(
        boost::bind(&P2PServer::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&P2PServer::onMessage, this, _1, _2, _3));
}
/*
void P2PServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
  const string& connection = req.getHeader("Connection");
  bool close = connection == "close" ||
    (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
  HttpResponse response(close);
  httpCallback_(req, &response);
  Buffer buf;
  response.appendToBuffer(&buf);
  conn->send(&buf);
  if (response.closeConnection())
  {
    conn->shutdown();
  }
}
*/



