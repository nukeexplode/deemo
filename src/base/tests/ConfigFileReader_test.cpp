#include "ConfigFileReader.h"

#include <string.h>
#include <assert.h>

int main() {
    CConfigFileReader config("../../etc/test.conf");
    assert(config.getConfigName("test1") == "1");
    assert(config.getConfigName("test2") == "1");
    assert(config.getConfigName("test3") == "1");

    return 0;
}