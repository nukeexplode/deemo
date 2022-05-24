#include <assert.h>

#include <muduo/net/EventLoop.h>

#include "ConfigFileReader.h"
#include "Singleton.h"

#include "MsgServer.h"
#include "LoginClientManager.h"
#include "ProxyClientManager.h"
#include "RouteClientManager.h"

using muduo::net::EventLoop;

EventLoop g_main_loop;

int main() {
    CConfigFileReader config("../etc/msgserver.conf");

    assert(Singleton<LoginClientManager>::Instance().init() == 0);
    assert(Singleton<ProxyClientManager>::Instance().init() == 0);
    assert(Singleton<RouteClientManager>::Instance().init() == 0);

    std::string listenip = config.getConfigName("listenip");
    uint16_t port = static_cast<uint16_t>(std::stoi(config.getConfigName("port")));
    assert(Singleton<MsgServer>::Instance().init(listenip.c_str(), port, &g_main_loop));

    g_main_loop.loop();
}