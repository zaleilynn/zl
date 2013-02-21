#ifndef _LYNN_WORKER_CONFIG_H_
#define _LYNN_WORKER_CONFIG_H_ 

#include "include/config.h"
#include "include/singleton.h"

class WorkerConfig : public Config {
public:
   int32_t CheckCompleteness();
};

typedef Singleton<WorkerConfig> WorkerConfigI;
#endif

