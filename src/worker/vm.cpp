#include <fstream>
#include <sstream>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <sys/stat.h>
#include <sys/types.h>
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

static Logger logger = Logger::getInstance("worker");

//一个类共用一个配置
string VM::m_xml_template = "";

//Init需要使用Task的一些信息
VM::VM(const VMInfo& info){
    m_info = info;
    m_state = VM_WAIT;
}

//不能用了
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
    conf_file << "[vm_worker]" << endl;
    conf_file << "os = " << m_info.os << endl;
    conf_file << "ip = " << m_info.ip << endl;
    conf_file << "port = " << 1000 << endl;
    conf_file << "vm_id = " << m_id << endl;
    conf_file << "worker_endpoint = " << ResourceManagerI::Instance()->GetEndpoint() << endl;
    conf_file << "heartbeat_interval = " << 15 << endl;
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
    buffer << m_info.vcpu;
    xml_copy.replace(pos, strlen("T_VCPU"), buffer.str());

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
    LOG4CPLUS_DEBUG(logger, xml_copy);
    return 0;
}

int32_t VM::Execute() {
    //就不保存连接吧
    virConnectPtr conn = virConnectOpen("qemu:///system");
    if(conn == NULL) {
        LOG4CPLUS_ERROR(logger, "fails to open connection to qemu:///system");
        return 1;
    }

    virDomainPtr vm_ptr = virDomainCreateXML(conn, m_xml.c_str(), 0);

    if(!vm_ptr) {
        virErrorPtr error = virGetLastError();
        LOG4CPLUS_ERROR(logger, error->message);
        virConnectClose(conn);
        return 1;
    }

    virDomainFree(vm_ptr);
    virConnectClose(conn);
    return 0;
}

void VM::VMFailed(){
    StateEventPtr event(new FailedEvent(m_info.id, ""));
    StateEventBufferI::Instance()->PushBack(event); 
    WriteLocker locker(m_lock);
    m_state = VM_FAIL;
}

void VM::VMFinished(){
    StateEventPtr event(new FinishedEvent(m_info.id));
    StateEventBufferI::Instance()->PushBack(event);
    WriteLocker locker(m_lock);
    m_state = VM_FINISH;
}

void VM::VMStarted(){
    StateEventPtr event(new StartedEvent(m_info.id));
    StateEventBufferI::Instance()->PushBack(event);
    WriteLocker locker(m_lock);
    m_state = VM_RUN;
}
