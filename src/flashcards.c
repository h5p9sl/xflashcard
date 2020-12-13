#include "flashcards.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FlashcardEntry_t new_flashcard_entry(const char* question, const char* answer) {
    FlashcardEntry_t entry;
    entry.question = strdup(question);
    entry.answer = strdup(answer);
    return entry;
}

bool parseFile(struct FLASHCARD_CTX* ctx, const char* fileData, size_t fileSize) {
    char* buf = strdup(fileData);
    char* line = strtok(buf, "\n");
    unsigned line_number = 1;

    while (line != NULL) {
        // make sure line is at least 3 characters long
        // and the line is not commented out
        if (strlen(line) >= 3 && line[0] != '#') {
            // make sure we have a seperator in our entry
            if (strchr(line, ':') != NULL) {
                int seperator_index = 0;

                for (int i = 0; i < strlen(line); i++) {
                    switch (line[i]) {
                        case '\\':
                            // Remove escape character token from string
                            strcpy(&line[i], &line[i + 1]);
                            break;
                        case ':':
                            seperator_index = i;
                            break;
                    }
                }

                if (seperator_index != 0) {
                    char* seperator = &line[seperator_index];
                    *seperator = '\0';

                    FlashcardEntry_t entry = new_flashcard_entry(&line[0], seperator + 1);

                    ctx->num_entries += 1;
                    ctx->entries = reallocarray(ctx->entries, sizeof(entry), ctx->num_entries);
                    if (ctx->entries == NULL) {
                        perror("reallocarray");
                    } else {
                        ctx->entries[ctx->num_entries - 1] = entry;
                    }
                } else {
                    printf("An unknown error has occured on definition #%u: \"%s\"\n: could not find seperator", line_number, line);
                }
            } else {
                printf("Syntax error on definition #%u: \"%s\"\n", line_number, line);
            }
        }

        line = strtok(NULL, "\n");
        line_number += 1;
    }

    free(buf);
    return true;
}

bool loadFlashcardData(struct FLASHCARD_CTX* ctx, const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "\"%s\": ", fileName);
        perror("fopen");
        return false;
    }

    // Get file size
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Create buffer for the file
    void* buffer = malloc(size);

    fread(buffer, 1, size, file);
    fclose(file);

    // Parse file
    bool result = parseFile(ctx, (char*)buffer, size);

    free(buffer);
    return result;
}

