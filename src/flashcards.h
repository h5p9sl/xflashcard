#ifndef _FLASHCARDS_H_
#define _FLASHCARDS_H_
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char* question;
    char* answer;
} FlashcardEntry_t;

struct FLASHCARD_CTX {
    size_t num_entries;
    FlashcardEntry_t* entries;
};

/*
    Initializes FLASHCARD_CTX struct with data loaded from the file
*/
bool loadFlashcardData(struct FLASHCARD_CTX* ctx, const char* fileName);

#endif

