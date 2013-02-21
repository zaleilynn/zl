#ifndef _LYNN_INCLUDE_EVENT_H_
#define _LYNN_INCLUDE_EVENT_H_

#include <stdint.h>
#include <string>

using std::string;

class Event {
public:
     Event() {
         m_error_code = 0;
         m_retry_times = 0;
     }
     virtual int32_t Handle() = 0;
     virtual string GetType() = 0;
     virtual int32_t HandleError() = 0;
     int32_t GetErrorCode() {
         return m_error_code;
     }
protected:
     int32_t m_error_code;
     int32_t m_retry_times;
};
#endif

