#ifndef _LYNN_TOOLS_CONTEXT_H_
#define _LYNN_TOOLS_CONTEXT_H_

#include <string>
#include <stdint.h>

using std::string;

class Context {
public:
   Context(const string& conf_file) : m_conf(conf_file) {};
   virtual ~Context() {};
   virtual int32_t Parse() = 0;
protected:
   string m_conf;
};

#endif

