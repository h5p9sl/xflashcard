#ifndef _MAIN_C_
#define _MAIN_C_
#include <stdbool.h>
#include <stdio.h>

#define DEBUG_IO program_arguments.debug
#define DEBUG_PRINTF(s...) if (DEBUG_IO != (FILE*)NULL) fprintf(DEBUG_IO, s)

struct MAIN_H_PROGRAM_ARGUMENTS {
    FILE* debug;
    int argc;
    char** argv;
    bool reverse;
    char** input;
};

extern struct MAIN_H_PROGRAM_ARGUMENTS program_arguments;

#endif

