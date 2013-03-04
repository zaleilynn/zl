#ifndef _LYNN_MASTER_MACHINE_H_
#define _LYNN_MASTER_MACHINE_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include <classad/classad.h>

#include "include/proxy.h"

using boost::shared_ptr;
using std::string;

class Machine  {
public:
    explicit Machine(const MachineInfo& info);
    ~Machine() throw() {};
    void LogInfo() const;
    string GetEndpoint(){
        return m_machine_info.endpoint; 
    }
    bool IsMatch(ClassAd ad);
 
    double GetRankValue() const {
        return m_rank_value;
    }
    void ForwardEvent();
private:
    MachineInfo m_machine_info;
    ClassAd m_ad;
    double m_rank_value; 
};

typedef shared_ptr<Machine> MachinePtr;
#endif
