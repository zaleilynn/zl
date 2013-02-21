#ifndef _LYNN_INCLUDE_ATTRIBUTES_H_
#define _LYNN_INCLUDE_ATTRIBUTES_H_

#include <string>

using std::string;

//这个会多次出现，为了防止typo和修改方便
static const string ATTR_NEED_VCPU = "need_vcpu";
static const string ATTR_ALL_VCPU = "all_vcpu";
static const string ATTR_AVAIL_VCPU = "avail_vcpu";
static const string ATTR_NEED_MEMORY = "need_memory";
static const string ATTR_ALL_MEMORY = "all_memory";
static const string ATTR_AVAIL_MEMORY = "avail_memory";
//注意classad本身用了ATTR_RANK定义
static const string ATTR_TASK_RANK = "task_rank";
static const string ATTR_TASK_REQUIREMENT = "task_requirement";
static const string ATTR_VC = "virtual_cluster";
static const string ATTR_CPU_USAGE = "cpu_usage";

static const string EXP_TASK_RANK = "1.0*" + ATTR_NEED_VCPU + "/" + ATTR_AVAIL_VCPU + " > 1.0*" + ATTR_NEED_MEMORY + "/" + ATTR_AVAIL_MEMORY + " ? 1.0*" + ATTR_NEED_VCPU + "/" + ATTR_AVAIL_VCPU + " : 1.0*" + ATTR_NEED_MEMORY + "/" + ATTR_AVAIL_MEMORY;
static const string EXP_TASK_REQUIREMENT = ATTR_NEED_VCPU + "<=" + ATTR_AVAIL_VCPU + " && " + ATTR_NEED_MEMORY + "<=" + ATTR_AVAIL_MEMORY;

#endif
