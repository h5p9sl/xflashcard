#include "flashcards.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool parseFile(struct FLASHCARD_CTX* ctx, char* fileData, size_t fileSize) {
    char** lines = NULL;
    size_t lines_size = 0; 

    // Initialize context to default values
    ctx->num_questions = 0;
    ctx->questions = NULL;
    ctx->answers = NULL;

    // Replace all newlines with null characters
    // And create an array of lines
    {
        char* currentLine = fileData;
        for (size_t i = 0; i < fileSize; i++) {
            if (fileData[i] == '\n') {
                fileData[i] = '\0';
                // Check if the line is AT LEAST 3 characters long
                // Check if the line is commented out
                if ((fileData + i - currentLine >= 3) &&
                    (*currentLine != '#')) {
    
                    // Push line onto array
                    lines_size++;
                    lines = realloc(lines, lines_size * sizeof(char*));
                    if (lines == NULL) {
                        puts("realloc(): OUT OF MEMORY");
                    }
                    lines[lines_size - 1] = currentLine;
                }
                // Set the current line pointer to the next line
                currentLine = fileData + i + 1;
            }
        }
    }

    // Fill context struct with information
    // (Seperate questions from answers)
    {
        for (size_t i = 0; i < lines_size; i++) {
            bool syntaxError = true;
            bool escape = false;
            size_t* escape_chars = (size_t*)NULL;
            size_t escape_chars_size = (size_t)0;
            char* currentLine = lines[i];

            for (size_t j = 0; j < strlen(lines[i]); j++) {
                if (escape == true) {
                    escape = false;
                }
                // If current character is an escape character...
                else if (currentLine[j] == '\\') {
                    escape = true;
                    // Mark this escape character for deletion
                    escape_chars = realloc(escape_chars, ++escape_chars_size * sizeof(size_t));
                    escape_chars[escape_chars_size - 1] = j;
                }
                else if (currentLine[j] == ':') {
                    syntaxError = false;
                    currentLine[j] = '\0';
                    char* question = currentLine;
                    char* answer = currentLine + j + 1;
                    // Now we have our question and answer.
                    // Push back pointers to strings
                    ctx->num_questions++;
                    ctx->questions = realloc(ctx->questions, ctx->num_questions * sizeof(char*));
                    ctx->answers = realloc(ctx->answers, ctx->num_questions * sizeof(char*));
                    // Create copies of strings and store them in the context
                    char* question_cpy = (char*)malloc(strlen(question) - escape_chars_size);
                    char* answer_cpy = (char*)malloc(strlen(answer));
                    // Manually copy question to question_cpy
                    // and ignore escape characters
                    {
                        size_t x = 0;
                        for (size_t k = 0; k < strlen(question) + 1; k++) {
                            bool f = false;
                            for (size_t l = 0; l < escape_chars_size; l++) {
                                if (k == escape_chars[l]) f = true;
                            }
                            if (f != true) {
                                question_cpy[x++] = question[k];
                            }
                        }
                    }
                    strcpy(answer_cpy, answer);
                    ctx->questions[ctx->num_questions - 1] = question_cpy;
                    ctx->answers[ctx->num_questions - 1] = answer_cpy;
                }
            }
            if (syntaxError) {
                printf("Syntax error on definition #%li: \"%s\"\n", i, currentLine);
            }
        }
    }

    return true;
}

bool loadFlashcardData(struct FLASHCARD_CTX* ctx, const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
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
    return parseFile(ctx, (char*)buffer, size);
}

