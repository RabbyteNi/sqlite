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

// command -> check if meta cmd (yes, goto meta; no, continue) -> go to prepare statement
// command starts with dot is meta-command
// two states
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

// other commands go into prepare process
// two status
typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

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

/**
 * get meta command from inputBuffer and execute
 * @param from inputBuffer get command
 * @return META_COMMAND_UNRECOGNIZED_COMMAND when not ".exit"
 */
MetaCommandResult doMetaCommand(InputBuffer* inputBuffer) {
    if (strcmp(inputBuffer->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
} Statement;

/**
 * Read command from inputBuffer, recognize the command and prepare accordingly
 * @param from inputBuffer get command
 * @param statement the type of the command
 * @return if preparation is successful
 */
PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strncmp(inputBuffer->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void executeStatement(Statement* statement) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            printf("This is an insert cmd.\n");
            break;
        case STATEMENT_SELECT:
            printf("This is a select cmd.\n");
            break;
    }
}

int main(int argc, char* argv[]) {
    InputBuffer* inputBuffer = newInputBuffer();
    while (true) {
        printPrompt();
        readInput(inputBuffer);
        // process meta cmd
        if (inputBuffer->buffer[0] == '.') {
            switch (doMetaCommand(inputBuffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'\n", inputBuffer->buffer);
                    continue;
            }
        }
        // process other commands
        Statement statement;
        switch (prepareStatement(inputBuffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized command '%s'\n", inputBuffer->buffer);
                continue;
        }
        executeStatement(&statement);
        printf("Executed\n");
    }
}
