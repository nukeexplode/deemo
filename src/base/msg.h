using ConnectionCallback = muduo::net::ConnectionCallback;
using MessageCallback = muduo::net::MessageCallback;
using CloseCallback = muduo::net::CloseCallback;
using TcpConnectionPtr = muduo::net::TcpConnectionPtr;
using Buffer = muduo::net::Buffer;
using EventLoop = muduo::net::EventLoop;
using TcpClient = muduo::net::TcpClient;
using InetAddress = muduo::net::InetAddress;
using TcpConnection = muduo::net::TcpConnection;   
using Timestamp = muduo::Timestamp;
using EventLoopThread = muduo::net::EventLoopThread;
using EventLoopThreadPool = muduo::net::EventLoopThreadPool;

struct chat_msg_header {
    int length;		  // package length
    int reversed;     // 保留
};