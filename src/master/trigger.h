#ifndef _LYNN_MASTER_TRIGGER_H_
#define _LYNN_MASTER_TRIGGER_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "master/executor_pool.h"

using boost::shared_ptr;
using std::string;

class Trigger {
public:
    Trigger(const string& name, int32_t value, int period) :
        m_name(name),
        m_value_threshold(value),
        m_period_threshold(period),
        m_start_time(0),
        m_is_triggered(false) {}
    Trigger() : m_value_threshold(0), 
                m_period_threshold(0), 
                m_start_time(0),
                m_is_triggered(false) {}
    void Action(const ExecutorPoolPtr& epp);
    virtual bool Condition() = 0;
    virtual bool Action() = 0;
private:
    string m_name;
    int32_t m_value_threshold;
    double m_period_threshold;
    time_t m_start_time;
    bool m_is_triggered;
};

typedef shared_ptr<Trigger> TriggerPtr;

class IdleTrigger : public Trigger {
public:
    IdleTrigger(int64_t id): m_id(id) {};
    bool Condition(const ExecutorPoolPtr& epp);
    bool Action(const ExecutorPoolPtr& epp);
private:
    int64_t m_id;
};

class CpuTrigger : public Trigger {
public:
    CpuTrigger(int32_t value = 50, double period = 1) : 
        Trigger("cpu", value, period) {}
    bool Condition(const ExecutorPoolPtr& epp);
    bool Action(const ExecutorPoolPtr& epp);
private:
};

#endif
