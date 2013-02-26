#include <fstream>
#include <sstream>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "worker/driver_manager.h"

using std::ifstream;
using std::stringstream;
using log4cplus::Logger;
using lynn::ReadLocker;
using lynn::WriteLocker;

static Logger logger = Logger::getInstance("worker");

int32_t DriverManager::AddDriver(const string& type, const string& vm_xml_location) {
    DriverPtr driver(new Driver());
    driver->type = type;
    ifstream file(vm_xml_location.c_str());
    if(!file) { 
         LOG4CPLUS_ERROR(logger, "cannot open file " + vm_xml_location);
         return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    driver->xml_template = buffer.str();
    file.close();

    if(type == "kvm" || type == "qemu") {
        driver->conn = virConnectOpen("qemu:///system");
    } else {
        LOG4CPLUS_ERROR(logger, "wrong type " << type);
        return 1; 
    }

    if(driver->conn == NULL) {
        LOG4CPLUS_ERROR(logger, "cannot open connection to " << type);
        return 1;
    }

    WriteLocker locker(m_lock);
    m_driver_pool[type] = driver;
    return 0;
}

//这个是个开放的接口
//可能是有点不安全，但是暂时就这么样吧
DriverPtr DriverManager::Get(const string& type) {
    ReadLocker locker(m_lock);
    map<string, DriverPtr>::iterator it = m_driver_pool.find(type);
    if(it != m_driver_pool.end()) {
        return it->second;
    }
    //没有找到就返回一个空指针 
    return DriverPtr();   
}

void DriverManager::DeleteDriver(const string& type) {
    WriteLocker locker(m_lock);
    map<string, DriverPtr>::iterator it = m_driver_pool.find(type);
    if(it != m_driver_pool.end()) {
       virConnectClose(it->second->conn);
       map.earse(it);
    }
}
