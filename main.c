#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "flashcards.h"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        struct FLASHCARD_CTX ctx;
        loadFlashcardData(&ctx, argv[1]);

        srand(time(NULL));

        while (true) {
            int rng = rand();
            int index = rng % ctx.num_questions;

            printf("? %s\n... ", ctx.questions[index]);
            char c = getchar();
            printf("%s\n\n", ctx.answers[index]);
        }

    }
    else {
        printf("Usage: %s <flashcard file>\n", argv[0]);
    }
    return 0;
}

