#ifndef _LYNN_WORKER_VM_H_
#define _LYNN_WORKER_VM_H_

#include <boost/shared_ptr.hpp>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <string>
#include "include/proxy.h"

using boost::shared_ptr;
using std::string;

struct VMInfo {
    int64_t id;
    string vc_name;
    string os;
    int32_t vcpu;
    int32_t memory;
    string ip;
};

class VM {
public:
    VM(const VMInfo& info);
    int32_t Init();
    int32_t Execute();
    void VMFailed();
    void VMFinished();
    void VMStarted();

    string GetId(){
        return m_id;
    }

private:
    int32_t CopyImage();
    int32_t CloneImage();
    VMInfo m_info;
    string m_id;
    string m_dir;
    string m_img;
    string m_xml;
    string m_iso;
    static string m_xml_template;
};

typedef shared_ptr<VM> VMPtr;
#endif
