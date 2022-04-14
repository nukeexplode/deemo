#include "Singleton.h"

#include <iostream>
#include <thread>
#include <memory>
#include <vector>

class Test {
public:
    Test() {
        std::cout << "construct" << std::endl;
    }

    ~Test() {
        std::cout << "destruct" << std::endl;
    }

    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;
};

int main() {
    Singleton<Test>::Instance();
    std::vector<std::unique_ptr<std::thread>> threads;
    for (int i = 0; i < 100; i++) {
        std::unique_ptr<std::thread> t(new std::thread([](){ Singleton<Test>::Instance(); }));
        threads.push_back(std::move(t));
    }
    

    for (auto& t : threads) {
        t->join();
    }

    return 0;
}