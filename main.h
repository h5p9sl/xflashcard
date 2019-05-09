#ifndef _MAIN_C_
#define _MAIN_C_
#include <stdbool.h>

struct {
    int argc;
    char** argv;

    bool reverse;
} program_arguments;

struct CmdArgument {
    const char* cmd;
    const char* alias;
    const char* desc;
    const void* callback;
};

#endif

