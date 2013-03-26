#ifndef _LYNN_TOOLS_ADD_CONTEXT_H_
#define _LYNN_TOOLS_ADD_CONTEXT_H_

#include <string>
#include "include/config.h"
#include "include/proxy.h"

using std::string;

class AddContext : public Config {
public:
    int32_t CheckCompleteness();
    VCInfo GetVCInfo();
};

#endif
