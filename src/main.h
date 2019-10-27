#ifndef _MAIN_C_
#define _MAIN_C_
#include <stdbool.h>
#include <stdio.h>

#define DEBUG_IO program_arguments.debug
#define DEBUG_PRINTF(s...) if (DEBUG_IO != (FILE*)NULL) fprintf(DEBUG_IO, s)

struct {
    FILE* debug;
    int argc;
    char** argv;
    bool reverse;
    char** input;
} program_arguments;

#endif

