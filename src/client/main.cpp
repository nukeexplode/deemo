#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>

#include "RegClient.h"

using muduo::net::EventLoopThread;

int main(int argc, char* argv[])
{
    LOG_INFO << "pid = " << getpid();
    EventLoopThread loopThread;
    InetAddress serverAddr("127.0.0.1", 20000);

    RegClient client(loopThread.startLoop(), serverAddr);
    client.connect();
    std::string line;
    for (;;) {
        std::string username, password;
        std::cout << "username: ";
        std::cin >> username; 
        std::cout << "password: ";
        std::cin >> password;

        User::RegReq req;
        req.set_username(username);
        req.set_password(password);
        client.send(&req);
    }
    client.disconnect();
}