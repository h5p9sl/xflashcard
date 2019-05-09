#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "flashcards.h"
#include "main.h"

void reverse_arg();
void help_arg();

static const struct CmdArgument args[] = {
    { "-h", "--help", "Displays list of arguments that can be passed into the program.", help_arg },
    { "-r", "--reverse", "Displays answers before questions. (ex. 1) \"204\", 2) \"What is 102*2?\").", reverse_arg }
};


void print_usage() {
    printf("Usage: %s [OPTIONS] <FILE>\n", program_arguments.argv[0]);
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

void parseArguments(int argc, char* argv[]) {
    bool arg_found = false;

    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < sizeof(args) / sizeof(args[0]); j++) {
            const struct CmdArgument* argument = &args[j];
            if (strcmp(argument->cmd, argv[i]) == 0 ||
                strcmp(argument->alias, argv[i]) == 0) {
                arg_found = true;
                
                ((void(*)(void))argument->callback)();
            }
        }
    }
}

int main(int argc, char* argv[]) {
    program_arguments.argc = argc;
    program_arguments.argv = argv;
    program_arguments.reverse = false;

    if (argc >= 2) {
        parseArguments(argc, argv);

        struct FLASHCARD_CTX ctx;
        loadFlashcardData(&ctx, argv[1]);

        srand(time(NULL));

        while (true) {
            int rng = rand();
            int index = rng % ctx.num_questions;

            if (program_arguments.reverse) {
                printf("? %s\n... ", ctx.answers[index]);
                char c = getchar();
                printf("%s\n\n", ctx.questions[index]);
            } else {
                printf("? %s\n... ", ctx.questions[index]);
                char c = getchar();
                printf("%s\n\n", ctx.answers[index]);
            }
        }
    }
    else {
        print_usage();
    }
    return 0;
}

