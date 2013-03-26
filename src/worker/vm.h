#ifndef _LYNN_WORKER_VM_H_
#define _LYNN_WORKER_VM_H_

#include <boost/shared_ptr.hpp>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <string>
#include "include/proxy.h"
#include "include/rwlock.h"

using boost::shared_ptr;
using std::string;
using lynn::RWLock;

enum VMState {
    VM_WAIT,
    VM_RUN,
    VM_FINISH,
    VM_FAIL,
};

struct VMInfo {
    int64_t id;
    string vc_name;
    string os;
    //这里的cpu为container的shares值
    //vcpu的个数统一为1，以后可以修改
    int32_t cpu;
    int32_t memory;
    string ip;
    int32_t IO;
};

class VM {
public:
    VM(const VMInfo& info);
    ~VM();
    int32_t Init();
    int32_t Execute();
    void VMFailed();
    void VMFinished();
    void VMStarted();
    string GetId(){
        return m_id;
    }

    VMState GetState();
    int32_t GetAllocatedCpu() {
        return m_info.cpu;
    }
    int32_t GetAllocatedMemory() {
        return m_info.memory;
    }
    //Executor使用的状态信息其实等于VM的状态信息
    ExecutorStat GetUsedResource();
    double GetCpuUsage();
    double GetMemoryUsage();
    double GetIOUsage();
    int32_t Recycle();
private:
    RWLock m_lock;
    int32_t CopyImage();
    int32_t CloneImage();
    VMInfo m_info;
    string m_id;
    string m_dir;
    string m_img;
    string m_xml;
    string m_iso;
    VMState m_state;
    virDomainPtr m_ptr;
    static virConnectPtr m_conn;
    static string m_xml_template;
};

typedef shared_ptr<VM> VMPtr;
#endif
