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

    bool IsTriggered() const {
        return m_is_triggered;
    }

    string GetName() const {
        return m_name;
    }
    
    double GetPeriod() const {
        return m_period_threshold;
    }

    int32_t GetValue() const {
        return m_value_threshold;
    }

    //给Idle Trigger 覆盖用的
    virtual int64_t GetId() {
        return -1;
    }
    
    void SetTriggerState(bool status) {
        m_is_triggered = status;
    }

    void Action(const ExecutorPoolPtr& epp);
    virtual bool Condition(const ExecutorPoolPtr& epp) = 0;
    virtual bool Operation(const ExecutorPoolPtr& epp) = 0;

private:
    string m_name;
    int32_t m_value_threshold;
    double m_period_threshold;
    time_t m_start_time;
    bool m_is_triggered;
};

class IdleTrigger : public Trigger {
public:
    IdleTrigger(int64_t id, int32_t value = 20, int period = 5): Trigger("Idle", value, period), m_id(id) {};
    int64_t GetId() {
        return m_id;
    }
    bool Condition(const ExecutorPoolPtr& epp);
    bool Operation(const ExecutorPoolPtr& epp);
private:
    int64_t m_id;
};

class OverloadTrigger : public Trigger {
public:
    OverloadTrigger(int32_t value = 80, double period = 1) : 
        Trigger("Overload", value, period), m_trigger_time(0){}
    bool Condition(const ExecutorPoolPtr& epp);
    bool Operation(const ExecutorPoolPtr& epp);
private:
    time_t m_trigger_time;
};

typedef shared_ptr<Trigger> TriggerPtr;
#endif
