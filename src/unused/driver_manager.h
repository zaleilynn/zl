#ifndef _LYNN_WORKER_DRIVER_MANAGER_H_
#define _LYNN_WORKER_DRIVER_MANAGER_H_

#include <string>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <boost/shared_ptr.hpp>
#include <map>
#include "include/singleton.h"
#include "include/rwlock.h"

using boost::shared_ptr;
using std::string;
using std::map;
using lynn::RWLock;

struct Driver {
    string type;
    virConnectPtr conn;
    string xml_template;
};

typedef shared_ptr<Driver> DriverPtr;

class DriverManager {
public:  
    int32_t AddDriver(const string& type, const string& xml_template);
    string GetXMLTemplate(const string& type);
    void Insert(const string& type, const DriverPtr& driver);
    void DeleteDriver(const string& type);
    //这个没有对内封闭，对外是开放的
    //若用libvirt，同样是要返回指针的，这个虽然不安全，但是就这样吧
    DriverPtr Get(const string& type);
private:
    RWLock m_lock;
    map<string, DriverPtr> m_driver_pool;    
};

typedef Singleton<DriverManager> DriverManagerI;
#endif
