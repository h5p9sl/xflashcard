#ifndef _FLASHCARDS_H_
#define _FLASHCARDS_H_
#include <stdbool.h>

struct FLASHCARD_CTX {
    unsigned num_questions;
    unsigned size_questions;
    char** questions;
    char** answers;
};

/*
    Initializes FLASHCARD_CTX struct with data loaded from the file
*/
bool loadFlashcardData(struct FLASHCARD_CTX* ctx, const char* fileName);

#endif

