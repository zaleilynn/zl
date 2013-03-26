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





typedef struct _ExecutorStat__isset {
  _ExecutorStat__isset() : vc_name(false), task_id(false), cpu_usage(false), memory_usage(false) {}
  bool vc_name;
  bool task_id;
  bool cpu_usage;
  bool memory_usage;
} _ExecutorStat__isset;

class ExecutorStat {
 public:

  static const char* ascii_fingerprint; // = "FB11FF08354CE6EBB7BFE0173A4D13DC";
  static const uint8_t binary_fingerprint[16]; // = {0xFB,0x11,0xFF,0x08,0x35,0x4C,0xE6,0xEB,0xB7,0xBF,0xE0,0x17,0x3A,0x4D,0x13,0xDC};

  ExecutorStat() : vc_name(), task_id(0), cpu_usage(0), memory_usage(0) {
  }

  virtual ~ExecutorStat() throw() {}

  std::string vc_name;
  int64_t task_id;
  double cpu_usage;
  double memory_usage;

  _ExecutorStat__isset __isset;

  void __set_vc_name(const std::string& val) {
    vc_name = val;
  }

  void __set_task_id(const int64_t val) {
    task_id = val;
  }

  void __set_cpu_usage(const double val) {
    cpu_usage = val;
  }

  void __set_memory_usage(const double val) {
    memory_usage = val;
  }

  bool operator == (const ExecutorStat & rhs) const
  {
    if (!(vc_name == rhs.vc_name))
      return false;
    if (!(task_id == rhs.task_id))
      return false;
    if (!(cpu_usage == rhs.cpu_usage))
      return false;
    if (!(memory_usage == rhs.memory_usage))
      return false;
    return true;
  }
  bool operator != (const ExecutorStat &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ExecutorStat & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ExecutorStat &a, ExecutorStat &b);

typedef struct _MachineInfo__isset {
  _MachineInfo__isset() : endpoint(false), cpu_usage(false), all_cpu(false), all_memory(false), avail_cpu(false), avail_memory(false), vm_list(false) {}
  bool endpoint;
  bool cpu_usage;
  bool all_cpu;
  bool all_memory;
  bool avail_cpu;
  bool avail_memory;
  bool vm_list;
} _MachineInfo__isset;

class MachineInfo {
 public:

  static const char* ascii_fingerprint; // = "EE689CEA7F4C7CF9B6BEF45B296FAA8C";
  static const uint8_t binary_fingerprint[16]; // = {0xEE,0x68,0x9C,0xEA,0x7F,0x4C,0x7C,0xF9,0xB6,0xBE,0xF4,0x5B,0x29,0x6F,0xAA,0x8C};

  MachineInfo() : endpoint(), cpu_usage(0), all_cpu(0), all_memory(0), avail_cpu(0), avail_memory(0) {
  }

  virtual ~MachineInfo() throw() {}

  std::string endpoint;
  double cpu_usage;
  int32_t all_cpu;
  int32_t all_memory;
  int32_t avail_cpu;
  int32_t avail_memory;
  std::vector<ExecutorStat>  vm_list;

  _MachineInfo__isset __isset;

  void __set_endpoint(const std::string& val) {
    endpoint = val;
  }

  void __set_cpu_usage(const double val) {
    cpu_usage = val;
  }

  void __set_all_cpu(const int32_t val) {
    all_cpu = val;
  }

  void __set_all_memory(const int32_t val) {
    all_memory = val;
  }

  void __set_avail_cpu(const int32_t val) {
    avail_cpu = val;
  }

  void __set_avail_memory(const int32_t val) {
    avail_memory = val;
  }

  void __set_vm_list(const std::vector<ExecutorStat> & val) {
    vm_list = val;
  }

  bool operator == (const MachineInfo & rhs) const
  {
    if (!(endpoint == rhs.endpoint))
      return false;
    if (!(cpu_usage == rhs.cpu_usage))
      return false;
    if (!(all_cpu == rhs.all_cpu))
      return false;
    if (!(all_memory == rhs.all_memory))
      return false;
    if (!(avail_cpu == rhs.avail_cpu))
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
  _VCInfo__isset() : name(false), quota(false), os(false), type(false), cpu(false), memory(false), IO(false), app(false) {}
  bool name;
  bool quota;
  bool os;
  bool type;
  bool cpu;
  bool memory;
  bool IO;
  bool app;
} _VCInfo__isset;

class VCInfo {
 public:

  static const char* ascii_fingerprint; // = "740EBE59832D377153B5F814B9A31324";
  static const uint8_t binary_fingerprint[16]; // = {0x74,0x0E,0xBE,0x59,0x83,0x2D,0x37,0x71,0x53,0xB5,0xF8,0x14,0xB9,0xA3,0x13,0x24};

  VCInfo() : name(), quota(0), os(), type(), cpu(0), memory(0), IO(0), app() {
  }

  virtual ~VCInfo() throw() {}

  std::string name;
  double quota;
  std::string os;
  std::string type;
  int32_t cpu;
  int32_t memory;
  int32_t IO;
  std::string app;

  _VCInfo__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_quota(const double val) {
    quota = val;
  }

  void __set_os(const std::string& val) {
    os = val;
  }

  void __set_type(const std::string& val) {
    type = val;
  }

  void __set_cpu(const int32_t val) {
    cpu = val;
  }

  void __set_memory(const int32_t val) {
    memory = val;
  }

  void __set_IO(const int32_t val) {
    IO = val;
  }

  void __set_app(const std::string& val) {
    app = val;
  }

  bool operator == (const VCInfo & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(quota == rhs.quota))
      return false;
    if (!(os == rhs.os))
      return false;
    if (!(type == rhs.type))
      return false;
    if (!(cpu == rhs.cpu))
      return false;
    if (!(memory == rhs.memory))
      return false;
    if (!(IO == rhs.IO))
      return false;
    if (!(app == rhs.app))
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
  _TaskInfo__isset() : vc_name(false), id(false), os(false), type(false), cpu(false), memory(false), IO(false), ip(false) {}
  bool vc_name;
  bool id;
  bool os;
  bool type;
  bool cpu;
  bool memory;
  bool IO;
  bool ip;
} _TaskInfo__isset;

class TaskInfo {
 public:

  static const char* ascii_fingerprint; // = "5192E837BB31FBCCCF773ADF6AFB483E";
  static const uint8_t binary_fingerprint[16]; // = {0x51,0x92,0xE8,0x37,0xBB,0x31,0xFB,0xCC,0xCF,0x77,0x3A,0xDF,0x6A,0xFB,0x48,0x3E};

  TaskInfo() : vc_name(), id(0), os(), type(), cpu(0), memory(0), IO(0), ip() {
  }

  virtual ~TaskInfo() throw() {}

  std::string vc_name;
  int64_t id;
  std::string os;
  std::string type;
  int32_t cpu;
  int32_t memory;
  int32_t IO;
  std::string ip;

  _TaskInfo__isset __isset;

  void __set_vc_name(const std::string& val) {
    vc_name = val;
  }

  void __set_id(const int64_t val) {
    id = val;
  }

  void __set_os(const std::string& val) {
    os = val;
  }

  void __set_type(const std::string& val) {
    type = val;
  }

  void __set_cpu(const int32_t val) {
    cpu = val;
  }

  void __set_memory(const int32_t val) {
    memory = val;
  }

  void __set_IO(const int32_t val) {
    IO = val;
  }

  void __set_ip(const std::string& val) {
    ip = val;
  }

  bool operator == (const TaskInfo & rhs) const
  {
    if (!(vc_name == rhs.vc_name))
      return false;
    if (!(id == rhs.id))
      return false;
    if (!(os == rhs.os))
      return false;
    if (!(type == rhs.type))
      return false;
    if (!(cpu == rhs.cpu))
      return false;
    if (!(memory == rhs.memory))
      return false;
    if (!(IO == rhs.IO))
      return false;
    if (!(ip == rhs.ip))
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
