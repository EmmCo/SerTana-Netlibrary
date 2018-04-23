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
#include <SerTana/net/P2P/P2PClient.h>
#include <SerTana/net/EventLoop.h>

using namespace SerTana;
using namespace SerTana::net;

void P2PClient::Connect_Peer(struct p2p_msg_device_info_t *pdevice_info)
{
    assert(pdevice_info != NULL);

    struct sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);

    memset(&peer_addr, 0, addr_len);
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr = pdevice_info->ip_addr;
    peer_addr.sin_port = pdevice_info->port;

    InetAddress _LocalAddr(P2P_TRANSFER_CLIENT_PORT);
    InetAddress _PeerAddr (peer_addr);

    TcpClientPtr _clientpeer = TcpClientPtr(new TcpClient( loop_,
                                                           _PeerAddr,
                                                          _LocalAddr,
                                                          "Connect to Peer"));
    peerclient_ = _clientpeer;


    peerclient_->setConnectionCallback(
        boost::bind(&P2PClient::onPeerConnection, this, _1));
    peerclient_->setMessageCallback(
        boost::bind(&P2PClient::onMessagePeer, this, _1, _2, _3));

    peerclient_->connect();
}

void P2PClient::onMessageServer(const TcpConnectionPtr &conn, Buffer *buf, TimestampTool receiveTime)
{

   struct p2p_msg_head_t            *prequest = NULL;
   struct p2p_msg_device_info_t *pdevice_info = NULL;
   int msg_head_len    = sizeof(struct p2p_msg_head_t);
   int device_info_len = sizeof(struct p2p_msg_device_info_t);

   prequest=(p2p_msg_head_t *)buf->peek();
   buf->retrieve(msg_head_len);

   switch (prequest->cmd)
    {
      case P2P_TRANSFER_PUNCH_HOLE:                 // Someone wanna connect with me
      case P2P_TRANSFER_QUERY_DEVICE_INFO_RESPONSE: // we wanna connect with another
        pdevice_info=(p2p_msg_device_info_t *)buf->peek();
        buf->retrieve(device_info_len);

        serclient_->disconnect();

        Connect_Peer(pdevice_info);
      break;

      default:
       break;
    }
}


void P2PClient::onMessagePeer(const TcpConnectionPtr &conn, Buffer *buf, TimestampTool receiveTime)
{
   int jk =-2;
}
void P2PClient::onServeConnection(const TcpConnectionPtr& conn)
{
    struct p2p_msg_head_t  pmsg  ;
    struct p2p_msg_ping_t *pping = NULL;
    int total_len = sizeof(struct p2p_msg_head_t) + sizeof(struct p2p_msg_ping_t);

    pmsg.magic         = P2P_TRANSFER_MAGIC;
    pmsg.cmd_len       = sizeof(struct p2p_msg_ping_t);
    pmsg.src_device_sn = device_sn;
    pmsg.cmd           = P2P_TRANSFER_PING;

    pping               = (struct p2p_msg_ping_t *)(pmsg.cmd_data);
    pping->device_sn    = device_sn;
    pping->network_type = NP_PORT_RESTRICTED_CONE_NAT;

    conn->send(&pmsg,total_len);//send ping
}
void P2PClient::onPeerConnection(const TcpConnectionPtr& conn)
{
    struct p2p_msg_head_t  pmsg  ;
    struct p2p_msg_ping_t *pping = NULL;
    int total_len = sizeof(struct p2p_msg_head_t) + sizeof(struct p2p_msg_ping_t);

    pmsg.magic         = P2P_TRANSFER_MAGIC;
    pmsg.cmd_len       = sizeof(struct p2p_msg_ping_t);
    pmsg.src_device_sn = device_sn;
    pmsg.cmd           = P2P_TRANSFER_PING;

    pping               = (struct p2p_msg_ping_t *)(pmsg.cmd_data);
    pping->device_sn    = device_sn;
    pping->network_type = NP_PORT_RESTRICTED_CONE_NAT;

    //conn->send(&pmsg,total_len);//send ping
}

int test()
{
    EventLoop _loop;

    InetAddress _LocalAddr (P2P_TRANSFER_CLIENT_PORT);
    InetAddress _ServerAddr(P2P_TRANSFER_SERVER_IP,P2P_TRANSFER_SERVER_PORT);

    P2PClient _p2pclient(&_loop,
                         _LocalAddr,
                         _ServerAddr,
                          12345,
                         "Listen to Peer",
                         "Connect to Server");

    _p2pclient.setThreadNum(1);
    _p2pclient.listen();
    _p2pclient.connect();
    _loop.loop();
}
void P2PClient::Punch_Cmd(uint64_t test_peer_device_sn)//Send Punch Hole cmd to Server
{
    struct p2p_msg_head_t  prequest  , *presponse = NULL;
    struct p2p_msg_device_info_t *pquery_device_info = NULL;
    int total_len = sizeof(struct p2p_msg_head_t) + sizeof(struct p2p_msg_device_info_t);

    prequest.magic         = P2P_TRANSFER_MAGIC;
    prequest.src_device_sn = device_sn;
    prequest.cmd_len       = sizeof(struct p2p_msg_device_info_t);
    prequest.cmd           = P2P_TRANSFER_QUERY_DEVICE_INFO_REQUEST;

    pquery_device_info = (struct p2p_msg_device_info_t *)(prequest.cmd_data);
    pquery_device_info->device_sn = test_peer_device_sn;

    if(serclient_->connection_->connected())
       serclient_->connection_->send(&prequest,total_len);
}

P2PClient::P2PClient(EventLoop*      loop    ,
                     InetAddress & LocalAddr ,
                     InetAddress & ServerAddr,
                     uint64_t      _device_sn,
                     const string& servename,
                     const string& clientname
                      ):server_   (new TcpServer(loop,LocalAddr,servename,TcpServer::kReusePort)),
                        serclient_(new TcpClient(loop,ServerAddr,LocalAddr,clientname)),
                        device_sn(_device_sn)
{
    serclient_->setConnectionCallback(
        boost::bind(&P2PClient::onServeConnection, this, _1));
    serclient_->setMessageCallback(
        boost::bind(&P2PClient::onMessageServer, this, _1, _2, _3));

    server_->setConnectionCallback(
        boost::bind(&P2PClient::onPeerConnection, this, _1));
    server_->setMessageCallback(
        boost::bind(&P2PClient::onMessagePeer, this, _1, _2, _3));
}
