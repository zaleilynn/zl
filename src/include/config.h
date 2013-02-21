#ifndef LYNN_INCLUDE_CONFIG_H_
#define LYNN_INCLUDE_CONFIG_H_ 

#include <map>
#include <string>

#include "include/rapidxml.hpp"
#include "include/rapidxml_utils.hpp"

using std::string;
using std::map;
using rapidxml::file;
using rapidxml::xml_document;
using rapidxml::xml_node;
using rapidxml::xml_attribute;

class Config {
public:
   int32_t Init(const string& conf_file) {
       xml_document<> doc;
        try {
            file<> fdoc(conf_file.c_str());
            doc.parse<0>(fdoc.data());
        } catch (rapidxml::parse_error& ex) {
            return 1;
        } catch (std::runtime_error& ex) {
            return 1;
        }
        for(xml_node<> *node = doc.first_node(); node; node = node->next_sibling()){
            m_attr_map[node->name()] = node->value();
        }
        return CheckCompleteness();
   }

   string Get(const string& name) {
       map<string, string>::iterator it = m_attr_map.find(name);
       if (it != m_attr_map.end())
           return it->second;
       else
           return "";
   }

   virtual int32_t CheckCompleteness() = 0;

private:
   map<string, string> m_attr_map;
};

#endif

