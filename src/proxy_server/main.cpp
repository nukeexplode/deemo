#include <assert.h>

#include <muduo/net/EventLoop.h>

#include "ConfigFileReader.h"
#include "Singleton.h"
#include "ProxyServer.h"

using EventLoop = muduo::net::EventLoop;

EventLoop g_main_loop;

int main() {
    CConfigFileReader config("../../proxyserver.conf");

    std::string listenip = config.getConfigName("listenip");
    uint16_t port = static_cast<uint16_t>(std::stoi(config.getConfigName("port")));
    assert(Singleton<ProxyServer>::Instance().init(listenip.c_str(), port, &g_main_loop));

    g_main_loop.loop();
}