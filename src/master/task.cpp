#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <classad/classad_distribution.h>

#include "master/task.h"
#include "master/identity.h"
#include "master/machine_pool.h"
#include "master/ip_pool.h"
#include "include/rpc.h"
#include "include/attributes.h"

using log4cplus::Logger;
using lynn::ReadLocker;
using lynn::WriteLocker;

static Logger logger = Logger::getInstance("master");

Task::Task(const TaskInfo& info): m_task_info(info) {
    //要不要处理一下错误情况？
    m_task_info.id = TaskIdentityI::Instance()->GetTaskId();
    m_state = TASK_WAIT;
    m_run_on = "";
    //要不要处理一下错误的情况?
    m_ad.InsertAttr(ATTR_NEED_VCPU, m_task_info.vcpu);
    m_ad.InsertAttr(ATTR_NEED_MEMORY, m_task_info.memory);
    ClassAdParser parser;
    //rank的算法, 选择比例最大的
    ExprTree* expr = parser.ParseExpression(EXP_TASK_RANK);
    m_ad.Insert(ATTR_TASK_RANK, expr);
    //requirement
    ExprTree* re_expr = parser.ParseExpression(EXP_TASK_REQUIREMENT);
    m_ad.Insert(ATTR_TASK_REQUIREMENT, re_expr);
    m_ad.InsertAttr(ATTR_VC, m_task_info.vc_name);
}

TaskState Task::GetTaskState() {
    ReadLocker locker(m_lock);
    return m_state; 
}

void Task::SetTaskState(TaskState ts) {
    WriteLocker locker(m_lock);
    m_state = ts;
}

//这个是调度的核心
//调用match
bool Task::AssignTask() {
    WriteLocker locker(m_lock);
    string rtn = MachinePoolI::Instance()->AssignTask(m_ad);
    if(rtn.empty()){
        LOG4CPLUS_ERROR(logger, "no match machine for " << m_task_info.id);
        return false;      
    }

    //给虚拟机分配ip
    string ip = IpPoolI::Instance()->GetNextIp();
    if(ip.empty()) {
        LOG4CPLUS_ERROR(logger, "no more ip for " << m_task_info.id);
        return false;
    }
    m_task_info.ip = ip;

    LOG4CPLUS_INFO(logger, "match task " << m_task_info.id << " to " << rtn);
    //比较好的是AssignTask和启动task同样在锁的状态下
    //这里其实有一个race condition在这里面，machine的一些资源可能被重复的分配，但是因为这里是单线程处理task，暂时不会有问题
    //若为多线程处理task，这里需要做额外的考虑
    //也可以把资源标记为占用，最后分配失败了再把资源给还回去 
    try {
        Proxy<WorkerClient> proxy = Rpc<WorkerClient, WorkerClient>::GetProxy(rtn);
        if(!proxy().StartTask(m_task_info)){
            LOG4CPLUS_ERROR(logger, "start task error " << m_task_info.id);
            return false;
        }
    } catch (TException &tx) {
        LOG4CPLUS_ERROR(logger, "start task error " << tx.what());
        return false;
    }
    
    m_state = TASK_RUN;
    m_run_on = rtn;
    return true;
}
