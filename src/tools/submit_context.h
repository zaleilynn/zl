#ifndef _LYNN_TOOLS_SUBMIT_CONTEXT_H_
#define _LYNN_TOOLS_SUBMIT_CONTEXT_H_

#include <string>
#include "include/config.h"
#include "include/proxy.h"

using std::string;

class SubmitContext : public Config {
public:
    int32_t CheckCompleteness();
    TaskInfo GetTaskInfo();
};

#endif

