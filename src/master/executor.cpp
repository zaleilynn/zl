#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/executor.h"
#include "master/config.h"

using lynn::WriteLocker;
using lynn::ReadLocker;

using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

void Executor::AddStatInfo(const ExecutorStat& stat){
    WriteLocker locker(m_lock);
    m_stat_list.push_back(stat);
}

bool Executor::IsOverLoad(int32_t period, double cpu_usage){
    int32_t heartbeat = atoi(MasterConfigI::Instance()->Get("heartbeat_interval").c_str());
    uint32_t len = period/heartbeat;
    ReadLocker locker(m_lock);
    if( m_stat_list.size() < len) {
        return false;
    }
    uint32_t i = 0;
    double sum = 0;
    for(list<ExecutorStat>::reverse_iterator riter = m_stat_list.rbegin();
        riter != m_stat_list.rend() && i < len; ++riter, ++i) {
        sum += riter->cpu_usage;
    }
    double value = sum / len;
    LOG4CPLUS_DEBUG(logger, "IsOverLoad,value: " << value);
    return value >= cpu_usage;
}

bool Executor::IsIdle(int32_t period, double cpu_usage) {
    int32_t heartbeat = atoi(MasterConfigI::Instance()->Get("heartbeat_interval").c_str());
    uint32_t len = period/heartbeat;
    ReadLocker locker(m_lock);
    if(m_stat_list.size() < len) {
        return false;
    } 
    uint32_t i = 0;
    double sum = 0;
    for(list<ExecutorStat>::reverse_iterator riter = m_stat_list.rbegin();
        riter != m_stat_list.rend() && i < len; ++riter, ++i) {
        sum += riter->cpu_usage;
    }
    double value = sum / len;
    LOG4CPLUS_DEBUG(logger, "Idle,value: " << value);
    return value <= cpu_usage;
}
