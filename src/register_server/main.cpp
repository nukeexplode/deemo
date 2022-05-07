#include <assert.h>

#include <muduo/net/EventLoop.h>

#include "ConfigFileReader.h"
#include "Singleton.h"

#include "RegServer.h"
#include "ProxyClientManager.h"

using muduo::net::EventLoop;

EventLoop g_main_loop;

int main() {
    CConfigFileReader config("../etc/registerserver.conf");

    assert(Singleton<ProxyClientManager>::Instance().init() == 0);

    std::string listenip = config.getConfigName("listenip");
    uint16_t port = static_cast<uint16_t>(std::stoi(config.getConfigName("port")));
    assert(Singleton<RegServer>::Instance().init(listenip.c_str(), port, &g_main_loop));

    g_main_loop.loop();
}