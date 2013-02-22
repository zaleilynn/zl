#include <fstream>
#include <sstream>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "worker/vm.h"

using std::stringstream;
using std::ofstream;
using std::string;
using std::endl;
using log4cplus::Logger;

static Logger logger = Logger::getInstance("worker");

//Init需要使用Task的一些信息
VM::VM(const TaskInfo& info){
    m_info = info;
}

int32_t VM::Init() {
    string vc_dir = "/var/lib/lynn/" + m_info.vc_name;
    if(access(vc_dir.c_str(), F_OK) == -1) {
        if(mkdir(vc_dir.c_str(), 0755) != 0) {
           LOG4CPLUS_ERROR(logger, "cannot create virtual cluster dir");
           return 1;
        }
    }
  
    stringstream buffer;
    buffer <<  m_info.id;
    string vm_id = buffer.str();

    string vm_dir = vc_dir + vm_id;

    if(mkdir(vm_dir.c_str(), 0755) != 0) {
       LOG4CPLUS_ERROR(logger, "cannot create vm dir");
       return 1;
    }
 
    string img_template = "/var/lib/lynn/" + m_info.vm_info.os;
    if(access(img_template.c_str(), F_OK) == -1) {
        LOG4CPLUS_ERROR(logger, "template " << m_info.vm_info.os << " dose not exits");
        return 1;
    }

    //复制镜像
    //每次都拷贝，或者用母镜像,不拷贝?
    string cmd = "cp " + img_template + " " + vm_dir + "/" + vm_id + ".img";
    int32_t res = system(cmd.c_str());
    res = res >> 8;
    if(res != 0) {
        LOG4CPLUS_ERROR(logger, "cannot copy image template");
        return 1;
    }
    //准备配置文件
    ofstream conf_file((vm_dir + "/CONF").c_str());
    conf_file << "[vm_worker]" << endl;

    /*
    conf_file << "os = " << vm_info.os << endl;
    conf_file << "ip = " << vm_info.ip << endl;
    conf_file << "port = " << vm_info.port << endl;
    conf_file << "worker_endpoint = " << ResourceManagerI::Instance()->GetBridgeEndpoint() << endl;
    conf_file << "heartbeat_interval = " << 15 << endl;
    conf_file << "vm_id = " << vm_info.id;
    */

    conf_file.close();
    return 0;
}
