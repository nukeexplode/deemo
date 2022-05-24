#include <assert.h>

#include <muduo/net/EventLoop.h>

#include "ConfigFileReader.h"
#include "Singleton.h"

#include "MsgClientManager.h"
#include "RouteServer.h"

using muduo::net::EventLoop;

EventLoop g_main_loop;

int main() {
    CConfigFileReader config("../etc/routeserver.conf");

    assert(Singleton<MsgClientManager>::Instance().init() == true);
    assert(Singleton<UserManager>::Instance().init());

    std::string listenip = config.getConfigName("listenip");
    uint16_t port = static_cast<uint16_t>(std::stoi(config.getConfigName("port")));
    assert(Singleton<RouteServer>::Instance().init(listenip.c_str(), port, &g_main_loop));

    g_main_loop.loop();
}