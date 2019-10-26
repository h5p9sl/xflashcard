#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "flashcards.h"
#include "main.h"

// May not work for all terminals...
#define CLEAR_SCREEN() printf("\e[1;1H\e[2J")

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
    debug = stdout;
    DEBUG_PRINTF("Debug Mode is enabled.\n");
}

// overflow is a char* array with a size of 'argc'. All indices initialized as NULL
void parseArguments(int argc, char* argv[], char* overflow[], int* overflow_count) {
    bool arg_found = false;
    *overflow_count = 0;

    for (int i = 0; i < argc; i++) {
        program_arguments.current_arg = i;
        for (int j = 0; j < sizeof(args) / sizeof(args[0]); j++) {
            const struct CmdArgument* argument = &args[j];
            if (strcmp(argument->cmd, argv[i]) == 0 ||
                strcmp(argument->alias, argv[i]) == 0) {
                arg_found = true;
                
                ((void(*)(void))argument->callback)();
            }
        }
        if (!arg_found) {
            overflow[(*overflow_count)++] = argv[i];
        }
    }
}

void pause() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void swap(int* a, int* b) {
    int v_a, v_b;
    // Swap the values of a and b
    v_a = *a; v_b = *b;
    *a = v_b;
    *b = v_a;
}

void shuffleIntArray(int* array, unsigned len) {
    for (int i = len - 1; i > 0; --i) {
        swap(&array[i], &array[rand() % (i + 1)]);
    }
}

int main(int argc, char* argv[]) {
    program_arguments.argc = argc;
    program_arguments.argv = argv;
    program_arguments.reverse = false;
    program_arguments.input = (char**)malloc(argc * sizeof(char*));

    if (argc >= 2) {
        int overflow_count;
        // Assume that the overflow arguments are input files
        char** overflow = program_arguments.input;
        memset(overflow, 0, (argc * sizeof(char*)));
        parseArguments(argc, argv, overflow, &overflow_count);

        struct FLASHCARD_CTX* ctx = (struct FLASHCARD_CTX*)malloc(sizeof(struct FLASHCARD_CTX));
        memset(ctx, 0, sizeof(struct FLASHCARD_CTX));
        for (int i = 1; i < overflow_count; i++) {
            DEBUG_PRINTF("overflow[%i] = \"%s\"\n", i, overflow[i]);
            loadFlashcardData(ctx, program_arguments.input[i]);
        }

        // Seed RNG
        srand(time(NULL));

        // Create an array of indexes
        //      We're going to use this array to define the order
        //      that we're going to ask the questions
        int* index = (int*)malloc(sizeof(int) * ctx->num_questions);
        while (ctx->num_questions > 0) {
            // Initialize data in index
            for (int i = 0; i < ctx->num_questions; i++) {
                index[i] = i;
            }

            shuffleIntArray(index, ctx->num_questions);

            for (int a = 0; a < ctx->num_questions; a++) {
                int i = index[a];
                char* qna[2] = { ctx->questions[i], ctx->answers[i] };

                // Clear the screen
                CLEAR_SCREEN();
                // Print question
                printf("? %s\n... ", qna[(int)(program_arguments.reverse)]);
                pause();
                // Print answer
                printf("%s\n\n", qna[(int)(!program_arguments.reverse)]);
                pause();
            }
        }

        free(overflow);
        free(index);
        free(ctx);
    }
    else {
        print_usage();
    }
    return 0;
}

