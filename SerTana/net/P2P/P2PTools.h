#ifndef SERTANA_P2PTOOLS_H
#define SERTANA_P2PTOOLS_H

#include <stdint.h>
#include <arpa/inet.h>


#define P2P_TRANSFER_MAGIC (0XBEEFDEAD1234)
#define P2P_TRANSFER_SERVER_IP     "127.0.0.1"
#define P2P_TRANSFER_SERVER_PORT     (8000)

#define P2P_TRANSFER_LISTEN_MAX_NUM	  (16)

#define P2P_TRANSFER_CLIENT_PORT	  (5152)


enum                       //NAT Type
{
  NP_UNKNOWN = 0         , //Unknown NAT_Net Type
  NP_PUBLIC_NETWORK      , //
  NP_FULL_CONE_NAT       ,
  NP_RESTRICTED_CONE_NAT ,
  NP_PORT_RESTRICTED_CONE_NAT,
  NP_SYMMETRIC_NAT,
};

enum                      //P2P CMD Type
{
   P2P_TRANSFER_UNKNOWN_CMD    = 0        ,
   P2P_TRANSFER_PING                      ,
   P2P_TRANSFER_QUERY_DEVICE_INFO_REQUEST ,
   P2P_TRANSFER_QUERY_DEVICE_INFO_RESPONSE,
   P2P_TRANSFER_PUNCH_HOLE                ,
   P2P_TRANSFER_CLOSE                     ,
};

struct p2p_msg_ping_t
{
    uint64_t  device_sn;
    int       network_type;

};

struct p2p_msg_device_info_t
{
    uint64_t       device_sn;
    struct in_addr ip_addr;
    uint16_t       port;
    int            network_type;
};

struct p2p_msg_head_t
{
    uint64_t  magic;
    uint64_t  msgid;
    uint64_t  src_device_sn;
    uint64_t  dst_device_sn;
    uint32_t  cmd_len;
    int       cmd;
    char      cmd_data[0];
};



#endif // SERTANA_P2PTOOLS_H
