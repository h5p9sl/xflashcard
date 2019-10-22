#ifndef _MAIN_C_
#define _MAIN_C_
#include <stdbool.h>

#define DEBUG_PRINTF(s...) if (debug != (FILE*)NULL) fprintf(debug, s)

FILE* debug = (FILE*)NULL;

struct {
    int argc;
    char** argv;
    int current_arg;
    bool reverse;
    char** input;
} program_arguments;

struct CmdArgument {
    const char* cmd;
    const char* alias;
    const char* desc;
    const void* callback;
};

#endif

