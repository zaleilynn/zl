#ifndef _LYNN_SRC_MASTER_SERVICE_H_
#define _LYNN_SRC_MASTER_SERVICE_H_

#include <string>
#include "include/proxy.h"

using std::string;

class MasterService : public MasterIf {
public:
    void Heartbeat(const MachineInfo& machine_info);
    //返回值暂时没有用
    int32_t AddVC(const VCInfo& vc_info);
};

#endif

