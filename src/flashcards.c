#include "flashcards.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool parseFile(struct FLASHCARD_CTX* ctx, char* fileData, size_t fileSize) {
    char* line = strtok(fileData, "\n");
    unsigned line_number = 1;

    while (line != NULL) {
		// make sure line is at least 3 characters long
        // the line is not commented out
        if (strlen(line) >= 3 && line[0] != '#') {
            // Fill context struct with information
            // (Seperate questions from answers)
            bool syntaxError = true;
            bool escape = false;
            size_t* escape_chars = (size_t*)NULL;
            size_t escape_chars_size = (size_t)0;

            for (size_t j = 0; j < strlen(line); j++) {
                if (escape == true) {
                    escape = false;
                }
                // If current character is an escape character...
                else if (line[j] == '\\') {
                    escape = true;
                    // Mark this escape character for deletion
                    escape_chars = realloc(escape_chars, ++escape_chars_size * sizeof(size_t));
                    escape_chars[escape_chars_size - 1] = j;
                }
                else if (line[j] == ':') {
                    syntaxError = false;
                    line[j] = '\0';
                    char* question = line;
                    char* answer = line + j + 1;
                    // Now we have our question and answer.
                    // Push back pointers to strings
                    ctx->num_questions++;
                    if (ctx->num_questions > ctx->size_questions) {
                        // We need to allocate more memory to store our pointers
                        while (ctx->size_questions <= ctx->num_questions) ctx->size_questions += 8;
                        ctx->questions = realloc(ctx->questions, ctx->size_questions * sizeof(char*));
                        ctx->answers = realloc(ctx->answers, ctx->size_questions * sizeof(char*));
                    }

                    // Create copies of strings and store them in the context
                    char* question_cpy = strdup(question);
                    char* answer_cpy = strdup(answer);
                    // Iterate through question_cpy and remove escape chars
                    {
                        bool is_escape;
                        size_t k, x = 0;
                        for (k = 0; k < strlen(question); k++) {
                            is_escape = false;
                            for (size_t l = 0; l < escape_chars_size; l++) {
                                if (k == escape_chars[l]) is_escape = true;
                            }
                            if (is_escape == false) question_cpy[x++] = question[k];
                        }
                        question_cpy[x] = '\0';
                    }
                    ctx->questions[ctx->num_questions - 1] = question_cpy;
                    ctx->answers[ctx->num_questions - 1] = answer_cpy;
                }
            }
            if (syntaxError) {
                printf("Syntax error on definition #%li: \"%s\"\n", line_number, line);
            }
            // Don't forget to free memory!
            free(escape_chars);
        }

        line = strtok(NULL, "\n");
        line_number += 1;
    }
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

