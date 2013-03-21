#include <stdint.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include "tools/add_context.h"

using std::string;

void ErrorMsg(char ** argv) {
    fprintf(stderr, "usage: %s -f [path]\n", argv[0]);
    fprintf(stderr, "\t-f: configuration file of virtual cluster\n");
}

int32_t main(int32_t argc, char ** argv) {
    string conf_file;
    if(argc == 3) {
        const char* flag = "-f";
        if(strcmp(argv[1], flag) == 0) {
           conf_file = argv[2];
        } else {
            ErrorMsg(argv);
            return -1;
        }
    } else {
        ErrorMsg(argv);
        return -1;
    }
    AddContext context(conf_file);
    if(context.Parse() !=0) {
        fprintf(stderr, "cannot parse conf_file\n");
        return -1;
    }
    return 0;
}
