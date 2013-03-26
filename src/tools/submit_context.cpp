#include <stdio.h>
#include <stdlib.h>
#include "tools/submit_context.h"
#include "include/rapidxml.hpp"
#include "include/rapidxml_utils.hpp"

using rapidxml::file;
using rapidxml::xml_document;
using rapidxml::xml_node;
using rapidxml::xml_attribute;

int32_t SubmitContext::CheckCompleteness() {
    const char* s[] = {
                       "master_endpoint",
                       "vc_name",
                       "type",
                       "os",
                       "cpu",
                       "IO",
                       "app"
                      };
    int32_t size = sizeof(s)/sizeof(s[0]);
    for(int32_t i = 0; i < size; i++) {
        if(Get(s[i]).empty()) {
            fprintf(stderr, "%s is not set\n",s[i]);
            return 1;
        }
    }
    return 0;
}

TaskInfo SubmitContext::GetTaskInfo() {
    TaskInfo task_info;
    task_info.vc_name = Get("vc_name");
    //这个以及下面的ip都会由master提交和调度的时候补全的
    task_info.id = -1;
    task_info.os = Get("os");
    task_info.type = Get("type");
    task_info.cpu = atoi(Get("cpu").c_str());
    task_info.memory = atoi(Get("memory").c_str());
    task_info.IO = atoi(Get("IO").c_str());
    task_info.ip = "";
    return task_info;
}

