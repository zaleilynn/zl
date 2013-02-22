#include <stdlib.h>

#include "master/ip_pool.h"
#include "master/config.h"

using lynn::WriteLocker;
using lynn::ReadLocker;

int32_t IpPool::Init(){
    string net = MasterConfigI::Instance()->Get("net");
    int32_t index = net.rfind(".");
    m_prefix = net.substr(0, index);
    string tmp = net.substr(index + 1);
    index = tmp.find("-");
    m_start = atoi(tmp.substr(0, index).c_str());
    m_cur = m_start;
    m_end = atoi(tmp.substr(index + 1).c_str());
    return 0;
}

bool IpPool::Match(const string& ip){
    int32_t index = ip.rfind(".");
    string prefix = ip.substr(0, index);
    if(prefix != m_prefix) {
        return false;
    }
   
}

bool IpPool::AllocateIp(const string& ip){
    WriteLocker locker(m_lock);
    if(m_ip_to_reuse.find(ip) != m_ip_to_reuse.end()) {
        m_ip_to_reuse.erase(ip);
        m_ip_pool.insert(ip);
        return true;
    }
    if(m_ip_pool.find(ip) != m_ip_pool.end()){
        return false;
    }
    m_ip_pool.insert(ip);
    return true;
}

bool IpPool::ReleaseIp(const string& ip){
    WriteLocker locker(m_lock);
    //根本没有分配，认为成功吧
    if(m_ip_pool.find(ip) == m_ip_pool.end()) {
        return true;
    }
    //在回收缓冲区里面
    if(m_ip_to_reuse.find(ip) != m_ip_to_reuse.end()){
        return true;
    }
    //已经被分配了
    m_ip_pool.erase(ip);
    m_ip_to_reuse.insert(ip);
    return true;
}

string IpPool::GetNextIp() {
    WriteLocker locker(m_lock);
    if(!m_ip_to_reuse.empty()){
       string rtn = *m_ip_to_reuse.begin();
       //先分配回收区中的ip
       //一个ip不会同时出现在两个set中
       m_ip_to_reuse.erase(rtn);
       m_ip_pool.insert(rtn);
       return rtn;
    }

    return "";
}
