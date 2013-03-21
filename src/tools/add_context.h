#ifndef _LYNN_TOOLS_ADD_CONTEXT_H_
#define _LYNN_TOOLS_ADD_CONTEXT_H_

#include <string>
#include "tools/context.h"
#include "include/proxy.h"

using std::string;

class AddContext : public Context{
public:
    AddContext(const string& conf_file) : Context(conf_file) {};
    ~AddContext(){};
    int32_t Parse();
    bool GetVCInfo(VCInfo& vc_info);
};

#endif
