include "../master/master.thrift"

service Worker {
    bool StartTask(1: master.TaskInfo info),
    bool KillTask(1: 
}
