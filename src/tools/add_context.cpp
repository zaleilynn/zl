#include <stdio.h>
#include <stdlib.h>
#include "tools/add_context.h"
#include "include/rapidxml.hpp"
#include "include/rapidxml_utils.hpp"

using rapidxml::file;
using rapidxml::xml_document;
using rapidxml::xml_node;
using rapidxml::xml_attribute;

int32_t AddContext::CheckCompleteness() {
    const char* s[] = {
                       "master_endpoint",
                       "name",
                       "quota",
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

VCInfo AddContext::GetVCInfo() {
    VCInfo vc_info; 
    vc_info.name = Get("name");
    vc_info.quota =  atof(Get("quota").c_str());
    vc_info.os = Get("os");
    vc_info.type = Get("type");
    vc_info.cpu = atoi(Get("cpu").c_str());
    vc_info.memory = atoi(Get("memory").c_str());
    vc_info.IO = atoi(Get("IO").c_str());
    vc_info.app = Get("app");
    return vc_info;
}
