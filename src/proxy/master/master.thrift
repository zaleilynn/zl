struct VMStat {
    1: string vc_name,
    2: i64 task_id,
    3: double used_vcpu,
    4: i32 used_memory,
}

struct MachineInfo {
    1: string endpoint,
    2: double cpu_usage,
    3: i32 all_vcpu,
    4: i32 all_memory,
    5: i32 avail_vcpu,
    6: i32 avail_memory,
    7: list<VMStat> vm_list,
}

struct VCInfo {
    1: string name,
    2: i32 vcpu,
    3: i32 memory,
    4: i32 quota,
}

struct TaskInfo {
    1: string vc_name,
    2: i64 id,
    3: i32 need_vcpu,
    4: i32 need_memory,
}

service Master {
    void Heartbeat(1: MachineInfo info),
    i32 AddVC(1: VCInfo vc_info),
}
