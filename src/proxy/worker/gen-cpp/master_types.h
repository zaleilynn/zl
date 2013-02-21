/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef master_TYPES_H
#define master_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>





typedef struct _VMStat__isset {
  _VMStat__isset() : vc_name(false), task_id(false), used_vcpu(false), used_memory(false) {}
  bool vc_name;
  bool task_id;
  bool used_vcpu;
  bool used_memory;
} _VMStat__isset;

class VMStat {
 public:

  static const char* ascii_fingerprint; // = "CA686CC0868D3556820A8027F5E9EE8D";
  static const uint8_t binary_fingerprint[16]; // = {0xCA,0x68,0x6C,0xC0,0x86,0x8D,0x35,0x56,0x82,0x0A,0x80,0x27,0xF5,0xE9,0xEE,0x8D};

  VMStat() : vc_name(), task_id(0), used_vcpu(0), used_memory(0) {
  }

  virtual ~VMStat() throw() {}

  std::string vc_name;
  int64_t task_id;
  double used_vcpu;
  int32_t used_memory;

  _VMStat__isset __isset;

  void __set_vc_name(const std::string& val) {
    vc_name = val;
  }

  void __set_task_id(const int64_t val) {
    task_id = val;
  }

  void __set_used_vcpu(const double val) {
    used_vcpu = val;
  }

  void __set_used_memory(const int32_t val) {
    used_memory = val;
  }

  bool operator == (const VMStat & rhs) const
  {
    if (!(vc_name == rhs.vc_name))
      return false;
    if (!(task_id == rhs.task_id))
      return false;
    if (!(used_vcpu == rhs.used_vcpu))
      return false;
    if (!(used_memory == rhs.used_memory))
      return false;
    return true;
  }
  bool operator != (const VMStat &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VMStat & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(VMStat &a, VMStat &b);

typedef struct _MachineInfo__isset {
  _MachineInfo__isset() : endpoint(false), cpu_usage(false), all_vcpu(false), all_memory(false), avail_vcpu(false), avail_memory(false), vm_list(false) {}
  bool endpoint;
  bool cpu_usage;
  bool all_vcpu;
  bool all_memory;
  bool avail_vcpu;
  bool avail_memory;
  bool vm_list;
} _MachineInfo__isset;

class MachineInfo {
 public:

  static const char* ascii_fingerprint; // = "2E26812E0ED4F46F8A3A9E75FA0B9AA9";
  static const uint8_t binary_fingerprint[16]; // = {0x2E,0x26,0x81,0x2E,0x0E,0xD4,0xF4,0x6F,0x8A,0x3A,0x9E,0x75,0xFA,0x0B,0x9A,0xA9};

  MachineInfo() : endpoint(), cpu_usage(0), all_vcpu(0), all_memory(0), avail_vcpu(0), avail_memory(0) {
  }

  virtual ~MachineInfo() throw() {}

  std::string endpoint;
  double cpu_usage;
  int32_t all_vcpu;
  int32_t all_memory;
  int32_t avail_vcpu;
  int32_t avail_memory;
  std::vector<VMStat>  vm_list;

  _MachineInfo__isset __isset;

  void __set_endpoint(const std::string& val) {
    endpoint = val;
  }

  void __set_cpu_usage(const double val) {
    cpu_usage = val;
  }

  void __set_all_vcpu(const int32_t val) {
    all_vcpu = val;
  }

  void __set_all_memory(const int32_t val) {
    all_memory = val;
  }

  void __set_avail_vcpu(const int32_t val) {
    avail_vcpu = val;
  }

  void __set_avail_memory(const int32_t val) {
    avail_memory = val;
  }

  void __set_vm_list(const std::vector<VMStat> & val) {
    vm_list = val;
  }

  bool operator == (const MachineInfo & rhs) const
  {
    if (!(endpoint == rhs.endpoint))
      return false;
    if (!(cpu_usage == rhs.cpu_usage))
      return false;
    if (!(all_vcpu == rhs.all_vcpu))
      return false;
    if (!(all_memory == rhs.all_memory))
      return false;
    if (!(avail_vcpu == rhs.avail_vcpu))
      return false;
    if (!(avail_memory == rhs.avail_memory))
      return false;
    if (!(vm_list == rhs.vm_list))
      return false;
    return true;
  }
  bool operator != (const MachineInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MachineInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MachineInfo &a, MachineInfo &b);

typedef struct _VCInfo__isset {
  _VCInfo__isset() : name(false), vcpu(false), memory(false), quota(false) {}
  bool name;
  bool vcpu;
  bool memory;
  bool quota;
} _VCInfo__isset;

class VCInfo {
 public:

  static const char* ascii_fingerprint; // = "9284B42FBE364FECA7AF8E76C1F488A2";
  static const uint8_t binary_fingerprint[16]; // = {0x92,0x84,0xB4,0x2F,0xBE,0x36,0x4F,0xEC,0xA7,0xAF,0x8E,0x76,0xC1,0xF4,0x88,0xA2};

  VCInfo() : name(), vcpu(0), memory(0), quota(0) {
  }

  virtual ~VCInfo() throw() {}

  std::string name;
  int32_t vcpu;
  int32_t memory;
  int32_t quota;

  _VCInfo__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_vcpu(const int32_t val) {
    vcpu = val;
  }

  void __set_memory(const int32_t val) {
    memory = val;
  }

  void __set_quota(const int32_t val) {
    quota = val;
  }

  bool operator == (const VCInfo & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(vcpu == rhs.vcpu))
      return false;
    if (!(memory == rhs.memory))
      return false;
    if (!(quota == rhs.quota))
      return false;
    return true;
  }
  bool operator != (const VCInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VCInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(VCInfo &a, VCInfo &b);

typedef struct _TaskInfo__isset {
  _TaskInfo__isset() : vc_name(false), id(false), need_vcpu(false), need_memory(false) {}
  bool vc_name;
  bool id;
  bool need_vcpu;
  bool need_memory;
} _TaskInfo__isset;

class TaskInfo {
 public:

  static const char* ascii_fingerprint; // = "816A2EDC53440EE9F1FD11DF1A5C0C51";
  static const uint8_t binary_fingerprint[16]; // = {0x81,0x6A,0x2E,0xDC,0x53,0x44,0x0E,0xE9,0xF1,0xFD,0x11,0xDF,0x1A,0x5C,0x0C,0x51};

  TaskInfo() : vc_name(), id(0), need_vcpu(0), need_memory(0) {
  }

  virtual ~TaskInfo() throw() {}

  std::string vc_name;
  int64_t id;
  int32_t need_vcpu;
  int32_t need_memory;

  _TaskInfo__isset __isset;

  void __set_vc_name(const std::string& val) {
    vc_name = val;
  }

  void __set_id(const int64_t val) {
    id = val;
  }

  void __set_need_vcpu(const int32_t val) {
    need_vcpu = val;
  }

  void __set_need_memory(const int32_t val) {
    need_memory = val;
  }

  bool operator == (const TaskInfo & rhs) const
  {
    if (!(vc_name == rhs.vc_name))
      return false;
    if (!(id == rhs.id))
      return false;
    if (!(need_vcpu == rhs.need_vcpu))
      return false;
    if (!(need_memory == rhs.need_memory))
      return false;
    return true;
  }
  bool operator != (const TaskInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TaskInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(TaskInfo &a, TaskInfo &b);



#endif
