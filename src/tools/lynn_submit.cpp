#include <string>
#include <stdint.h>
#include "tools/submit_context.h"
#include "include/proxy.h"
#include "include/rpc.h"

using std::string;

void ErrorMsg(char ** argv) {
    fprintf(stderr, "usage: %s -f [path]\n", argv[0]);
    fprintf(stderr, "\t-f: configuration file of virtual cluster\n");
}

void InfoMsg(const string& master_endpoint, const TaskInfo& task_info) {
    printf("add task to %s, task info:\n", master_endpoint.c_str());
    printf("vc name: %s\n", task_info.vc_name.c_str());
    printf("os: %s\n", task_info.os.c_str());
    printf("type: %s\n", task_info.type.c_str());
    printf("cpu: %d\n", task_info.cpu);
    printf("memory: %d\n", task_info.memory);
    printf("IO: %d\n", task_info.IO);
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
    SubmitContext context;
    if(context.Init(conf_file) !=0) {
        fprintf(stderr, "cannot parse conf_file\n");
        return -1;
    }
    TaskInfo task_info = context.GetTaskInfo();
    string master_endpoint = context.Get("master_endpoint");  
    InfoMsg(master_endpoint, task_info);
    Proxy<MasterClient> proxy = Rpc<MasterClient, MasterClient>::GetProxy(master_endpoint);
    proxy().SubmitTask(task_info); 
}
