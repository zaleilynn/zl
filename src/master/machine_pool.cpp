#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/machine_pool.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

using log4cplus::Logger;
static Logger logger = Logger::getInstance("master");

void MachinePool::Insert(const MachinePtr& machine) {
    string endpoint = machine->GetEndpoint();
    WriteLocker locker(m_lock);
    m_machine_pool[endpoint] = machine;
}

void MachinePool::MapToDo(MachineFunc func) {
    ReadLocker locker(m_lock);
    for(map<string, MachinePtr>::iterator it = m_machine_pool.begin();
        it != m_machine_pool.end(); ++it){
        func(it->second);
    }
}

//把这个移除出去有帮助于代码的统一性，但是存在一个方便性的问题？
string MachinePool::AssignTask(const ClassAd& ad){
    ReadLocker locker(m_lock);
    list<MachinePtr> candidate_machine;
    for(map<string, MachinePtr>::iterator it = m_machine_pool.begin();
        it != m_machine_pool.end(); ++it){  
        if(it->second->IsMatch(ad)) {
            LOG4CPLUS_INFO(logger, "match a machine");
            candidate_machine.push_back(it->second);
        }  
    }
 
    if(candidate_machine.size() <= 0){
        return "";
    }

    candidate_machine.sort(Compare);  
    return candidate_machine.front()->GetEndpoint();
}

bool MachinePool::Compare(const MachinePtr& left, const MachinePtr& right){
    return left->GetRankValue() < right->GetRankValue(); 
}

int32_t MachinePool::Size() {
    ReadLocker locker(m_lock); 
    return m_machine_pool.size();
}
