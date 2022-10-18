/**
 * a demo sqlite
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer * newInputBuffer() {
    InputBuffer * inputBuffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;
    return inputBuffer;
}

void printPrompt() {
    printf("db > ");
}

void readInput(InputBuffer * inputBuffer) {
    ssize_t bytesRead = getline(&(inputBuffer->buffer), &(inputBuffer->buffer_length), stdin);
    if (bytesRead <= 0) {
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }
    inputBuffer->input_length = bytesRead - 1;
    inputBuffer->buffer[bytesRead - 1] = 0;
}

void closeInputBuffer(InputBuffer* inputBuffer) {
    free(inputBuffer->buffer);
    free(inputBuffer);
}

int main(int argc, char* argv[]) {
    InputBuffer* inputBuffer = newInputBuffer();
    while (true) {
        printPrompt();
        readInput(inputBuffer);

        if (strcmp(inputBuffer->buffer, ".exit") == 0) {
            closeInputBuffer(inputBuffer);
            exit(EXIT_SUCCESS);
        }
        else {
            printf("Unrecognized command '%s'.\n", inputBuffer->buffer);
        };
    }
}
