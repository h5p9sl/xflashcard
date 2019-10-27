#include "arg.h"
#include "main.h" // for program_arguments

#include <stdlib.h>
#include <string.h>

void print_usage();
void reverse_arg();
void help_arg();
void input_arg();
void debug_arg();

static const struct CmdArgument args[] = {
    { "-h", "--help", "Displays list of arguments that can be passed into the program.", help_arg },
    { "-r", "--reverse", "Displays answers before questions. (ex. 1) \"204\", 2) \"What is 102*2?\").", reverse_arg },
    { "-d", "--debug", "Enables technical information output (Debug Mode).", debug_arg },
};


void print_usage() {
    printf("Usage: %s [OPTIONS] <FILE(S)>\n", program_arguments.argv[0]);
}

void reverse_arg() {
    puts("Reverse mode enabled.");
    program_arguments.reverse = true;
}

void help_arg() {
    print_usage();
    for (int i = 0; i < sizeof(args) / sizeof(args[0]); i++) {
        const struct CmdArgument* argument = &args[i];
        printf("%s    %s    %s\n", argument->cmd, argument->alias, argument->desc);
    }

    exit(0);
}

void debug_arg() {
    DEBUG_IO = stdout;
    DEBUG_PRINTF("Debug Mode is enabled.\n");
}

// overflow is a char* array with a size of 'argc'. All indices initialized as NULL
void parseArguments(int argc, char* argv[], char* overflow[], int* overflow_count) {
    bool arg_found = false;

    *overflow_count = 0;

    // Iterate through arguments supplied by the user
    for (int i = 0; i < argc; i++) {
        // Compare with all of the defined arguments
        for (int j = 0; j < sizeof(args) / sizeof(args[0]); j++) {
            const struct CmdArgument* argument = &args[j];
            if (strcmp(argument->cmd, argv[i]) == 0 ||
                    strcmp(argument->alias, argv[i]) == 0)
            {
                arg_found = true;
                ((void(*)(void))argument->callback)();
                // Break out of this loop; we don't need to keep
                // comparing, we already found a match.
                break;
            }
        }
        if (!arg_found) {
            overflow[(*overflow_count)++] = argv[i];
        }
    }
}

