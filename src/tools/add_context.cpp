#include <stdio.h>
#include "tools/add_context.h"
#include "include/rapidxml.hpp"
#include "include/rapidxml_utils.hpp"

using rapidxml::file;
using rapidxml::xml_document;
using rapidxml::xml_node;
using rapidxml::xml_attribute;

int32_t AddContext::Parse(){
    xml_document<> doc;
    try {
        file<> fdoc(m_conf.c_str());
    } catch (rapidxml::parse_error& ex) {
        fprintf(stderr, "parse error %s", ex.what());
        return -1;
    } catch (std::runtime_error& ex) { 
        fprintf(stderr, "parse error %s", ex.what());
        return -1;
    }
    return 0;
}

bool GetVCInfo(VCInfo&){
    return true;
}
