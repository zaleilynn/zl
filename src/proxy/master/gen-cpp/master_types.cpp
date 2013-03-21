/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "master_types.h"

#include <algorithm>



const char* ExecutorStat::ascii_fingerprint = "FB11FF08354CE6EBB7BFE0173A4D13DC";
const uint8_t ExecutorStat::binary_fingerprint[16] = {0xFB,0x11,0xFF,0x08,0x35,0x4C,0xE6,0xEB,0xB7,0xBF,0xE0,0x17,0x3A,0x4D,0x13,0xDC};

uint32_t ExecutorStat::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->vc_name);
          this->__isset.vc_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->task_id);
          this->__isset.task_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->cpu_usage);
          this->__isset.cpu_usage = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->memory_usage);
          this->__isset.memory_usage = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ExecutorStat::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ExecutorStat");

  xfer += oprot->writeFieldBegin("vc_name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->vc_name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("task_id", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->task_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("cpu_usage", ::apache::thrift::protocol::T_DOUBLE, 3);
  xfer += oprot->writeDouble(this->cpu_usage);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("memory_usage", ::apache::thrift::protocol::T_DOUBLE, 4);
  xfer += oprot->writeDouble(this->memory_usage);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ExecutorStat &a, ExecutorStat &b) {
  using ::std::swap;
  swap(a.vc_name, b.vc_name);
  swap(a.task_id, b.task_id);
  swap(a.cpu_usage, b.cpu_usage);
  swap(a.memory_usage, b.memory_usage);
  swap(a.__isset, b.__isset);
}

const char* MachineInfo::ascii_fingerprint = "EE689CEA7F4C7CF9B6BEF45B296FAA8C";
const uint8_t MachineInfo::binary_fingerprint[16] = {0xEE,0x68,0x9C,0xEA,0x7F,0x4C,0x7C,0xF9,0xB6,0xBE,0xF4,0x5B,0x29,0x6F,0xAA,0x8C};

uint32_t MachineInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->endpoint);
          this->__isset.endpoint = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->cpu_usage);
          this->__isset.cpu_usage = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->all_cpu);
          this->__isset.all_cpu = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->all_memory);
          this->__isset.all_memory = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->avail_cpu);
          this->__isset.avail_cpu = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->avail_memory);
          this->__isset.avail_memory = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->vm_list.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->vm_list.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += this->vm_list[_i4].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.vm_list = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t MachineInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("MachineInfo");

  xfer += oprot->writeFieldBegin("endpoint", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->endpoint);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("cpu_usage", ::apache::thrift::protocol::T_DOUBLE, 2);
  xfer += oprot->writeDouble(this->cpu_usage);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("all_cpu", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->all_cpu);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("all_memory", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->all_memory);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("avail_cpu", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->avail_cpu);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("avail_memory", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->avail_memory);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("vm_list", ::apache::thrift::protocol::T_LIST, 7);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->vm_list.size()));
    std::vector<ExecutorStat> ::const_iterator _iter5;
    for (_iter5 = this->vm_list.begin(); _iter5 != this->vm_list.end(); ++_iter5)
    {
      xfer += (*_iter5).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MachineInfo &a, MachineInfo &b) {
  using ::std::swap;
  swap(a.endpoint, b.endpoint);
  swap(a.cpu_usage, b.cpu_usage);
  swap(a.all_cpu, b.all_cpu);
  swap(a.all_memory, b.all_memory);
  swap(a.avail_cpu, b.avail_cpu);
  swap(a.avail_memory, b.avail_memory);
  swap(a.vm_list, b.vm_list);
  swap(a.__isset, b.__isset);
}

const char* VCInfo::ascii_fingerprint = "24F5E3FA9272C001062C433603F469B3";
const uint8_t VCInfo::binary_fingerprint[16] = {0x24,0xF5,0xE3,0xFA,0x92,0x72,0xC0,0x01,0x06,0x2C,0x43,0x36,0x03,0xF4,0x69,0xB3};

uint32_t VCInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          this->__isset.name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->quota);
          this->__isset.quota = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->os);
          this->__isset.os = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->type);
          this->__isset.type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->cpu);
          this->__isset.cpu = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->memory);
          this->__isset.memory = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->IO);
          this->__isset.IO = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->app);
          this->__isset.app = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t VCInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("VCInfo");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("quota", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->quota);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("os", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->os);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("cpu", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->cpu);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("memory", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->memory);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("IO", ::apache::thrift::protocol::T_I32, 7);
  xfer += oprot->writeI32(this->IO);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("app", ::apache::thrift::protocol::T_STRING, 8);
  xfer += oprot->writeString(this->app);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(VCInfo &a, VCInfo &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.quota, b.quota);
  swap(a.os, b.os);
  swap(a.type, b.type);
  swap(a.cpu, b.cpu);
  swap(a.memory, b.memory);
  swap(a.IO, b.IO);
  swap(a.app, b.app);
  swap(a.__isset, b.__isset);
}

const char* TaskInfo::ascii_fingerprint = "8C217E4194BBCC62D4E457735D96A34F";
const uint8_t TaskInfo::binary_fingerprint[16] = {0x8C,0x21,0x7E,0x41,0x94,0xBB,0xCC,0x62,0xD4,0xE4,0x57,0x73,0x5D,0x96,0xA3,0x4F};

uint32_t TaskInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->vc_name);
          this->__isset.vc_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->os);
          this->__isset.os = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->cpu);
          this->__isset.cpu = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->memory);
          this->__isset.memory = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->IO);
          this->__isset.IO = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->ip);
          this->__isset.ip = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t TaskInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("TaskInfo");

  xfer += oprot->writeFieldBegin("vc_name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->vc_name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("os", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->os);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("cpu", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->cpu);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("memory", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->memory);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("IO", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->IO);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ip", ::apache::thrift::protocol::T_STRING, 7);
  xfer += oprot->writeString(this->ip);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(TaskInfo &a, TaskInfo &b) {
  using ::std::swap;
  swap(a.vc_name, b.vc_name);
  swap(a.id, b.id);
  swap(a.os, b.os);
  swap(a.cpu, b.cpu);
  swap(a.memory, b.memory);
  swap(a.IO, b.IO);
  swap(a.ip, b.ip);
  swap(a.__isset, b.__isset);
}


