#ifndef _ARG_C_
#define _ARG_C_

struct CmdArgument {
    const char* cmd;
    const char* alias;
    const char* desc;
    const void* callback;
};

void print_usage();
void parseArguments(int argc, char* argv[], char* overflow[], int* overflow_count);

#endif

