#include <memory>
#include <string>

#include <muduo/net/TcpConnection.h>
#include <muduo/base/Logging.h>

#include "User.pb.h"

#include "codec.h"
#include "dispatcher.h"

using namespace muduo::net;

using Buffer = muduo::net::Buffer;

class TcpSession
{
public:
    TcpSession(const std::weak_ptr<TcpConnection>& tmpconn);
    ~TcpSession();

    TcpSession(const TcpSession& rhs) = delete;
    TcpSession& operator =(const TcpSession& rhs) = delete;

    std::shared_ptr<TcpConnection> getConnectionPtr()
    {
        if (m_conn.expired())
            return NULL;

        return m_conn.lock();
    }

    //send
    // void send(int32_t sid, int32_t cmd, int32_t seq, const std::string& data);
    // void send(int32_t sid, int32_t cmd, int32_t seq, const char* data, int32_t dataLength);
    // void send(int32_t sid, int32_t cmd, int32_t seq, ::google::protobuf::Any* proto);
    // void send(IM::Msg& msg)
    // void send(const std::string& p);
    // void send(const char* p, int32_t length);
    void send(google::protobuf::Message* message);

    ProtobufCodec* getProtoobufCodec() {
        return &m_codec;
    }
private:
    void sendPackage(const char* p, int32_t length);

protected:
    std::weak_ptr<TcpConnection> m_conn;
    ProtobufCodec m_codec;
    ProtobufDispatcher m_dispatcher;
};