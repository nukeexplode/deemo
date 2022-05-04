#include "TcpSession.h"

TcpSession::TcpSession(const std::weak_ptr<TcpConnection>& tmpconn) : m_conn(tmpconn)
{
    
}

TcpSession::~TcpSession()
{
    
}

// void TcpSession::send(int32_t sid, int32_t cmd, int32_t seq, const std::string& data)
// {
//     send(sid, cmd, seq, data.c_str(), data.length());
// }

// void TcpSession::send(uint32_t sid, uint32_t cmd, uint32_t seq, const char* data, int32_t dataLength)
// {
//     Buffer buf;
//     buf.append(data, dataLength); 

//     uint32_t length = buf.readableBytes();
//     buf.prependInt32(0);
//     buf.prependInt32(length);

//     sendPackage(buf.peek(), buf.readableBytes());
//     buf.retrieveAll();
// }

// void TcpSession::send(int32_t sid, int32_t cmd, int32_t seq, ::google::protobuf::Any* proto)
// {
//     Buffer buf;
//     IM::Msg msg;
//     msg.set_sid(sid);
//     msg.set_cmd(cmd);
//     msg.set_seq(seq);
//     msg.set_allocated_package(proto);

//     uint32_t msg_size = msg.ByteSize();
//     char* rp_msg = new char[msg_size];
//     if (!msg.SerializeToArray(rp_msg, msg_size))
//     {
//         LOG_ERROR << "Serialize failed";
//     }
//     buf.append(rp_msg, msg_size);

//     int32_t length = buf.readableBytes();
//     buf.prependInt32(0);
//     buf.prependInt32(length);

//     sendPackage(buf.peek(), buf.readableBytes());
//     buf.retrieveAll();
// }

// void TcpSession::send(IM::Msg& msg)
// {
//     Buffer buf;
//     uint32_t msg_size = msg.ByteSize();
//     char* rp_msg = new char[msg_size];
//     if (!msg.SerializeToArray(rp_msg, msg_size))
//     {
//         LOG_ERROR << "Serialize failed";
//     }
//     buf.append(rp_msg, msg_size);

//     int32_t length = buf.readableBytes();
//     buf.prependInt32(0);
//     buf.prependInt32(length);

//     sendPackage(buf.peek(), buf.readableBytes());
//     buf.retrieveAll();
// }

// // void TcpSession::send(const std::string& p)
// // {
// //     sendPackage(p.c_str(), p.length());
// // }

// // void TcpSession::send(const char* p, int32_t length)          
// // {
// //     sendPackage(p, length);
// // }

// void TcpSession::sendPackage(const char* p, int32_t length)
// {   
//     // std::string srcbuf(p, length);

//     // //TODO: 这些Session和connection对象的生命周期要好好梳理一下
//     // if (tmpConn_.expired())
//     // {
//     //     //FIXME: 出现这种问题需要排查
//     //     LOGE("Tcp connection is destroyed , but why TcpSession is still alive ?");
//     //     return;
//     // }
 
//     std::shared_ptr<TcpConnection> conn = m_conn.lock();
//     if (conn)
//     {   
//         conn->send(p, length);
//     }
// }

void TcpSession::send(google::protobuf::Message* message) {
    std::shared_ptr<TcpConnection> conn = m_conn.lock();
    if (conn)
    {   
        m_codec.send(conn, *message);
    }
}