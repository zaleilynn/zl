#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <classad/classad.h>
#include <classad/classad_distribution.h>
#include <vector>

#include "master/machine.h"
#include "include/attributes.h"
#include "master/vc_pool.h"

using std::vector;
using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

Machine::Machine(const MachineInfo& info): m_machine_info(info) {
    m_ad.InsertAttr(ATTR_CPU_USAGE, info.cpu_usage);
    m_ad.InsertAttr(ATTR_ALL_VCPU, info.all_vcpu);
    m_ad.InsertAttr(ATTR_ALL_MEMORY, info.all_memory);
    m_ad.InsertAttr(ATTR_AVAIL_VCPU, info.avail_vcpu);
    m_ad.InsertAttr(ATTR_AVAIL_MEMORY, info.avail_memory);
}

void Machine::LogInfo() const {
     LOG4CPLUS_INFO(logger, "machine regist:"
                    << " endpoint: " << m_machine_info.endpoint
                    << " vcpu: " << m_machine_info.all_vcpu
                    << " avail: " << m_machine_info.avail_vcpu
                    << " memory: " << m_machine_info.all_memory
                    << " avail: " << m_machine_info.avail_memory
                    << " cpu usage: " << m_machine_info.cpu_usage
                    );
}

bool Machine::IsMatch(ClassAd ad){
    //暂时没有上锁
    ad.alternateScope = &m_ad;
    bool is_match = false;
 
    //for debug
    string s;
    ClassAdXMLUnParser xml_unparser;
    xml_unparser.SetCompactSpacing(false);
    xml_unparser.Unparse(s, &ad);
    LOG4CPLUS_DEBUG(logger, "task \n" << s);
    xml_unparser.Unparse(s, &m_ad);
    LOG4CPLUS_DEBUG(logger, "machine \n" << s);

    ad.EvaluateAttrBool(ATTR_TASK_REQUIREMENT, is_match);
    if(!is_match) {
        return false;
    }

    //for debug
    ad.EvaluateAttrNumber(ATTR_TASK_RANK, m_rank_value);
    LOG4CPLUS_DEBUG(logger, "rank " << m_rank_value);
    return true;
}

//将其内部的VMStat信息转发到相应VC中去
//这个也可直接实现在Watcher中,但是可能就是在内存多存一份MachineInfo数据
void Machine::ForwardEvent() {
    for(vector<ExecutorStat>::iterator it = m_machine_info.vm_list.begin();
        it != m_machine_info.vm_list.end(); ++it) {
        VCPool::VCFunc func = bind(&VC::AddEvent, _1, *it); 
        VCPoolI::Instance()->FindToDo(it->vc_name, func); 
    }
} 
