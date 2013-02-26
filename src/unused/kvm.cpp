#include "worker/kvm.h"

//配置环境&&目录
int32_t KVM::Init() {
    string vc_dir = "/var/lib/lynn/" + m_info.vc_name;
    if(access(vc_dir.c_str(), F_OK) == -1) {
        if(mkdir(vc_dir.c_str(), 0755) != 0) {
           LOG4CPLUS_ERROR(logger, "cannot create virtual cluster dir");
           return 1;
        }
    }

    stringstream buffer;
    buffer <<  m_info.id;
    m_id = buffer.str();

    m_dir = vc_dir + m_id;

    if(access(m_dir.c_str(), F_OK) == -1){
        if(mkdir(m_dir.c_str(), 0755) != 0) {
           LOG4CPLUS_ERROR(logger, "cannot create vm dir");
           return 1;
        }
    }

    string img_template = "/var/lib/lynn/img/" + m_info.vm_info.os + ".img";
    if(access(img_template.c_str(), F_OK) == -1) {
        LOG4CPLUS_ERROR(logger, "template " << img_template << " dose not exits");
        return 1;
    }

    //复制镜像
    //每次都拷贝，或者用母镜像,不拷贝?
    string cmd = "cp " + img_template + " " + m_dir + "/" + m_id + ".img";
    int32_t res = system(cmd.c_str());
    res = res >> 8;
    if(res != 0) {
        LOG4CPLUS_ERROR(logger, "cannot copy image template");
        return 1;
    }
    
    //准备配置文件
    ofstream conf_file((m_dir + "/CONF").c_str());
    conf_file << "[vm_worker]" << endl;
    conf_file << "os = " << m_info.vm_info.os << endl;
    conf_file << "ip = " << m_info.vm_info.ip << endl;
    conf_file << "vm_id = " << m_id << endl;
    conf_file << "worker_endpoint = " << ResourceManagerI::Instance()->GetEndpoint() << endl;
    conf_file.close();
    cmd = "mkisofs -o " + m_dir + "/" + m_id + ".iso " + m_dir + "/CONF > /dev/null 2>&1";
    res = system(cmd.c_str());
    res = res >> 8;
    if(res != 0) {
        LOG4CPLUS_ERROR(logger, "cannot create conf iso file");
        return 1;
    }
    return 0;
}

int32_t KVM::Execute() {
    return 0;
}

