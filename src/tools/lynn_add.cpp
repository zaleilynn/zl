#include <stdint.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include "tools/add_context.h"
#include "include/proxy.h"
#include "include/rpc.h"

using std::string;

void ErrorMsg(char ** argv) {
    fprintf(stderr, "usage: %s -f [path]\n", argv[0]);
    fprintf(stderr, "\t-f: configuration file of virtual cluster\n");
}

void InfoMsg(const string& master_endpoint, const VCInfo& vc_info) {
    printf("add vc to %s, vc info:\n", master_endpoint.c_str());
    printf("name: %s\n", vc_info.name.c_str());
    printf("quota: %f\n", vc_info.quota);
    printf("os: %s\n", vc_info.os.c_str());
    printf("type: %s\n", vc_info.type.c_str());
    printf("cpu: %d\n", vc_info.cpu);
    printf("memory: %d\n", vc_info.memory);
    printf("IO: %d\n", vc_info.IO);
    printf("app: %s\n", vc_info.app.c_str());
}

int32_t main(int32_t argc, char ** argv) {
    string conf_file;
    if(argc == 3) {
        const char* flag = "-f";
        if(strcmp(argv[1], flag) == 0) {
           conf_file = argv[2];
        } else {
            ErrorMsg(argv);
            return -1;
        }
    } else {
        ErrorMsg(argv);
        return -1;
    }
    AddContext context;
    if(context.Init(conf_file) !=0) {
        fprintf(stderr, "cannot parse conf_file\n");
        return -1;
    }
    VCInfo vc_info = context.GetVCInfo();
    string master_endpoint = context.Get("master_endpoint");
    InfoMsg(master_endpoint, vc_info);  
    Proxy<MasterClient> proxy = Rpc<MasterClient, MasterClient>::GetProxy(master_endpoint);
    proxy().AddVC(vc_info);
    return 0;
}
