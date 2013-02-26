#include <stdint.h>
#include "include/proxy.h"
#include "include/rpc.h"

using std::string;

string master_endpoint = "127.0.0.1:9999";

int32_t main(int argc, char ** argv) {
    VCInfo info;
    info.name = "test";
    info.quota = 0.3;
    info.vcpu = 1;
    info.memory = 512; 
    info.os = "winxp";
    Proxy<MasterClient> proxy = Rpc<MasterClient, MasterClient>::GetProxy(master_endpoint);
    proxy().AddVC(info);
}

