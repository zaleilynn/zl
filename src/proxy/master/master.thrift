struct ExecutorStat {
    1: string vc_name,
    2: i64 task_id,
    3: double cpu_usage,
    4: double memory_usage,
}

struct MachineInfo {
    1: string endpoint,
    2: double cpu_usage, 
    3: i32 all_cpu,
    4: i32 all_memory,
    5: i32 avail_cpu,
    6: i32 avail_memory,
    7: list<ExecutorStat> vm_list,
}

struct VCInfo {
    1: string name,
    2: double quota,
    3: string os,
    4: string type,
    5: i32 cpu,
    6: i32 memory,
    7: i32 IO,
    8: string app,
}

struct TaskInfo {
    //虚拟集群名字
    1: string vc_name,
    //任务id号
    2: i64 id,
    3: string os,
    4: string type,
    5: i32 cpu,
    6: i32 memory,
    7: i32 IO,
    8: string ip,
}

service Master {
    i64 SubmitTask(1: TaskInfo info),
    void Heartbeat(1: MachineInfo info),
    i32 AddVC(1: VCInfo vc_info),
    i32 TaskStarted(1: i64 task_id),
    i32 TaskFinished(1: i64 task_id),
    i32 TaskFailed(1: i64 task_id),
}
