#include <fstream>
#include <sstream>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include "worker/vm.h"
#include "worker/resource_manager.h"
#include "worker/event.h"

using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::string;
using std::endl;
using log4cplus::Logger;
using lynn::WriteLocker;
using lynn::ReadLocker;

static Logger logger = Logger::getInstance("worker");

//一个类共用一个配置
string VM::m_xml_template = "";
//程序运行的时候会一直在的
virConnectPtr VM::m_conn = NULL;

//Init需要使用Task的一些信息
VM::VM(const VMInfo& info){
    m_info = info;
    m_state = VM_WAIT;
    m_ptr = NULL;
}

VM::~VM() {
    //m_ptr的初始化一定要做，要不可能m_ptr里面有值，虽然是一个没有用的值
    //进去了调用virDomainFree就会造成段错误
    if(m_ptr) {
        virDomainFree(m_ptr);
    }
}

//这个函数没有用了
//template里面需要修改为type = raw
//要加判断
int32_t VM::CopyImage() {
    string img_template = "/var/lib/lynn/img/" + m_info.os + ".img";
    if(access(img_template.c_str(), F_OK) == -1) {
        LOG4CPLUS_ERROR(logger, "template " << img_template << " dose not exits");
        return 1;
    }
    string cmd = "cp " + img_template + " " + m_img;
    int32_t res = system(cmd.c_str());
    res = res >> 8;
    if(res != 0) {
        LOG4CPLUS_ERROR(logger, "cannot copy image template");
        return 1;
    }
    return 0;
}

//增量克隆虚拟机
int32_t VM::CloneImage() {
    string img_template = "/var/lib/lynn/img/" + m_info.os + ".qco";
    if(access(img_template.c_str(), F_OK) == -1) {
        LOG4CPLUS_ERROR(logger, "template " << img_template << " dose not exits");
        return 1;
    }
    string cmd = "qemu-img create -b " + img_template + " -f qcow2 " + m_img + " > /dev/null 2>&1";
    int32_t res = system(cmd.c_str());
    res = res >> 8;
    if(res != 0) {
        LOG4CPLUS_ERROR(logger, "cannot clone image template");
        return 1;
    }
    return 0;
}

//配置环境&&目录
int32_t VM::Init() {
    //做成可配置的
    string work_dir = "/var/lib/lynn/";
    stringstream buffer;
    string cmd;
    int32_t res;
    buffer <<  m_info.id;
    m_id = buffer.str();
    m_dir = work_dir + m_info.vc_name + "_" + m_id;
    m_img = m_dir + "/" + m_id + ".img";
    m_iso = m_dir + "/" + m_id + ".iso";

    if(access(m_dir.c_str(), F_OK) == -1){
        if(mkdir(m_dir.c_str(), 0755) != 0) {
           LOG4CPLUS_ERROR(logger, "cannot create vm dir");
           return 1;
        }
    }

    if(CloneImage() != 0) {
        LOG4CPLUS_ERROR(logger, "cannot clone image");
        return 1;
    }
    
    //准备配置文件
    ofstream conf_file((m_dir + "/CONF").c_str());
    conf_file << "[agent]" << endl;
    conf_file << "ip = " << m_info.ip << endl;
    conf_file << "app = " << m_info.app << endl;
    conf_file.close();

    cmd = "mkisofs -o " + m_iso + " " + m_dir + "/CONF > /dev/null 2>&1";
    res = system(cmd.c_str());
    res = res >> 8;
    if(res != 0) {
        LOG4CPLUS_ERROR(logger, "cannot create conf iso file");
        return 1;
    }

    if(VM::m_xml_template.empty()) {
        ifstream file("/var/lib/lynn/img/kvm.xml");
        if(!file) {  
            LOG4CPLUS_ERROR(logger, "cannot read xml template file");
        }
        buffer.str("");
        buffer.clear();
        buffer << file.rdbuf();
        VM::m_xml_template = buffer.str();
    }
    string xml_copy = VM::m_xml_template;
 
    string::size_type pos = xml_copy.find("T_NAME");
    if(pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_NAME in vm template");
        return 1;
    }
    xml_copy.replace(pos, strlen("T_NAME"), m_id);

    pos = xml_copy.find("T_MEMORY");
    if( pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_MEMORY in vm template");
        return 1;
    }
    buffer.str("");
    buffer.clear();
    buffer << (m_info.memory * 1024);
    xml_copy.replace(pos, strlen("T_MEMORY"), buffer.str());
   
    pos = xml_copy.find("T_VCPU");
    if( pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_VCPU in vm template");
        return 1;
    }
    buffer.str("");
    buffer.clear();
    //vcpu以及cpu_shares目前是有联系的，对于kvm来说，CPU最好是个整数
    buffer << m_info.cpu;
    xml_copy.replace(pos, strlen("T_VCPU"), buffer.str());

    pos = xml_copy.find("T_CPU_SHARE");
    if( pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_CPU_SHARE in vm template");
        return 1;
    }
    buffer.str("");
    buffer.clear();
    // cpu 参数设置的是cpu的share值
    // 一个cpu对应的归一化参数是1024 
    buffer << m_info.cpu * 1024;
    xml_copy.replace(pos, strlen("T_CPU_SHARE"), buffer.str()); 

    pos = xml_copy.find("T_OUT_BOUND");
    if( pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_OUT_BOUND in vm template");
        return 1;
    }
    buffer.str("");
    buffer.clear();
    //目前In/Out IO都被同样的控制了
    buffer << m_info.IO;
    xml_copy.replace(pos, strlen("T_OUT_BOUND"), buffer.str());
 
    pos = xml_copy.find("T_IN_BOUND");
    if( pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_IN_BOUND in vm template");
        return 1;
    }
    buffer.str("");
    buffer.clear();
    buffer << m_info.IO;
    xml_copy.replace(pos, strlen("T_IN_BOUND"), buffer.str());

    pos = xml_copy.find("T_IMG_LOCATION");
    if( pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_IMG_LOCATION in vm template");
        return 1;
    }
    xml_copy.replace(pos, strlen("T_IMG_LOCATION"), m_img);

    pos = xml_copy.find("T_ISO_LOCATION");
    if( pos == string::npos) {
        LOG4CPLUS_ERROR(logger, "error in finding T_ISO_LOCATION in vm template");
        return 1;
    }
    xml_copy.replace(pos, strlen("T_ISO_LOCATION"), m_iso);
    m_xml = xml_copy;
    return 0;
}

int32_t VM::Execute() {
    if(VM::m_conn == NULL) {
        m_conn = virConnectOpen("qemu:///system");
        if(m_conn == NULL) {
            LOG4CPLUS_ERROR(logger, "fails to open connection to qemu:///system");
            return 1;
        }
    }
    m_ptr = virDomainCreateXML(m_conn, m_xml.c_str(), 0);

    if(!m_ptr) {
        virErrorPtr error = virGetLastError();
        LOG4CPLUS_ERROR(logger, error->message);
        return 1;
    }
    return 0;
}

void VM::VMFailed(){
    EventPtr event(new FailedEvent(m_info.id, ""));
    EventBufferI::Instance()->PushBack(event); 
    WriteLocker locker(m_lock);
    m_state = VM_FAIL;
}

void VM::VMFinished(){
    EventPtr event(new FinishedEvent(m_info.id));
    EventBufferI::Instance()->PushBack(event);
    WriteLocker locker(m_lock);
    m_state = VM_FINISH;
}

void VM::VMStarted(){
    EventPtr event(new StartedEvent(m_info.id));
    EventBufferI::Instance()->PushBack(event);
    WriteLocker locker(m_lock);
    m_state = VM_RUN;
}

VMState VM::GetState() {
    ReadLocker locker(m_lock);
    return m_state;
}

ExecutorStat VM::GetUsedResource() {
    ExecutorStat stat;
    stat.cpu_usage = GetCpuUsage();
    stat.memory_usage = GetMemoryUsage();
    stat.io_usage = GetIOUsage();
    stat.vc_name = m_info.vc_name;
    stat.task_id = m_info.id;
    return stat;
}

double VM::GetCpuUsage() {
/*
//下面计算的CPU偏小
    static virDomainInfo p_info;
    static timeval p_real_time;
    static bool first = true;
    virDomainInfo info;
    struct timeval real_time;
    if(virDomainGetInfo(m_ptr, &info) != 0) {
        LOG4CPLUS_ERROR(logger, "cannot get domain info!");
        return 0;
    }
    if(gettimeofday(&real_time, NULL) == -1) {
        LOG4CPLUS_ERROR(logger, "can get real time!");
        return 0;
    }
    if(first) {
       first = false;
       p_info = info;
       p_real_time = real_time;
       return 0.0;
    }
    //换算成微秒
    int32_t cpu_diff = (info.cpuTime - p_info.cpuTime) / 1000;
    int32_t real_diff = 1000000 * (real_time.tv_sec - p_real_time.tv_sec) +
                        (real_time.tv_usec - p_real_time.tv_usec);
    LOG4CPLUS_DEBUG(logger, "usage " << cpu_diff / (double) (real_diff));
    return cpu_diff / (double) (real_diff);
*/
    virDomainInfo info_s,info_e;
    struct timeval real_time_s, real_time_e;
    int cpu_diff, real_diff;
    float usage;

    if(virDomainGetInfo(m_ptr, &info_s) != 0) {
        LOG4CPLUS_ERROR(logger, "cannot get domain info!");
        return 0;
    }
    if(gettimeofday(&real_time_s, NULL) == -1) {
        LOG4CPLUS_ERROR(logger, "can get real time!");
        return 0;
    }

    //睡0.5秒，这个很可能出问题...延迟心跳进程的发送,尤其是虚拟机多的时候， TODO
    usleep(500000);

    if(virDomainGetInfo(m_ptr, &info_e) !=0) {
        LOG4CPLUS_ERROR(logger, "cannot get domain info!");
        return 0;
    }

    if(gettimeofday(&real_time_e, NULL) == -1) {
        LOG4CPLUS_ERROR(logger, "can get real time!");
        return 0;
    }
    //转换成微秒
    cpu_diff = (info_e.cpuTime - info_s.cpuTime) / 1000;
    //转换成微秒
    real_diff = 1000000 * (real_time_e.tv_sec - real_time_s.tv_sec) +
                    (real_time_e.tv_usec - real_time_s.tv_usec);
    usage = cpu_diff / (float) (real_diff);
    return usage; 
}

//TODO
double VM::GetMemoryUsage() {
    return 0;
}

//TODO
double VM::GetIOUsage() {
    return 0;
}

int32_t VM::Recycle() {
    return virDomainDestroy(m_ptr);
}
