#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "flashcards.h"
#include "main.h"
#include "arg.h"

// May not work for all terminals...
#define CLEAR_SCREEN() printf("\e[1;1H\e[2J")

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
    int overflow_count;
    struct FLASHCARD_CTX* ctx;
    {
        program_arguments.argc = argc;
        program_arguments.argv = argv;
        program_arguments.debug = (FILE*)NULL;
        program_arguments.reverse = false;
        program_arguments.input = (char**)malloc(argc * sizeof(char*));
    }

    // Has user supplied insufficient arguments?
    if (argc < 2) {
        print_usage();
        return 0;
    }

    // Assume that the overflow arguments are input files
    char** overflow = program_arguments.input;
    memset(overflow, 0, (argc * sizeof(char*)));
    parseArguments(argc, argv, overflow, &overflow_count);

    // Allocate memory for context
    ctx = (struct FLASHCARD_CTX*)malloc(sizeof(struct FLASHCARD_CTX));
    memset(ctx, 0, sizeof(struct FLASHCARD_CTX));

    // Load flashcard(s)
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

    // 1) Check if we actually have any questions
    // 2) Loop infinitely, asking the user questions
    while (ctx->num_questions > 0) {
        // Shuffle flashcards...
        for (int i = 0; i < ctx->num_questions; i++) index[i] = i;
        shuffleIntArray(index, ctx->num_questions);

        // Iterate through shuffled flashcards
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

    return 0;
}

