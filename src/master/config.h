#ifndef _LYNN_MASTER_CONFIG_H_
#define _LYNN_MASTER_CONFIG_H_ 

#include "include/config.h"
#include "include/singleton.h"

class MasterConfig : public Config {
public:
   int32_t CheckCompleteness();
};

typedef Singleton<MasterConfig> MasterConfigI;
#endif

