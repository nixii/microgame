
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "lexer.h"

// define the da
DA_DEFINE(ms_tokens, ms_token);

// lexer state
typedef struct {
    FILE *f;
    char *readBuf;
    size_t bufSize;
    ssize_t numRead;
    ssize_t curIdx;
} _lexer_state;

// tokenize
ms_token _tokenize_identifier(_lexer_state *state) {
    size_t start = state->curIdx;
    size_t len = 0;
    int endLoop = 0;

    // while there is a char
    while (state->curIdx < state->numRead && !endLoop) {

        // add the char
        switch (state->readBuf[state->curIdx]) {
            case 'A'...'Z':
            case 'a'...'z':
            case '0'...'9':
            case '_':
                len++;
                break;
            default:
                endLoop = 1;
                break;
        }
        state->curIdx++;
    }

    // handle booleans
    if (strncmp(state->readBuf + start, "true", len) == 0) {
        return (ms_token){
            .type = MS_TT_BOOL,
            .value = { .num = 1 }
        };
    } else if (strncmp(state->readBuf + start, "false", len) == 0) {
        return (ms_token){
            .type = MS_TT_BOOL,
            .value = { .num = 0 }
        };
    }

    // nil
    else if (strncmp(state->readBuf + start, "nil", len) == 0) {
        return (ms_token){
            .type = MS_TT_NIL
        };
    }

    // create the new string ident
    char *newString = strndup(state->readBuf + start, len);

    // create the token
    return (ms_token) {
        .type = MS_TT_IDENT,
        .value = { .chars = newString }
    };
}

// tokenize numbes or a vector
ms_token _tokenize_numbers(_lexer_state *state) {

    // up to 4 numbers in a row
    float numbers[4];
    int numNums = 0;
    int endLoop = 0;

    // find the numbers
    while (state->curIdx < state->numRead && !endLoop) {

        // get the char
        char c = state->readBuf[state->curIdx];
        char *endptr;

        // number buffer
        switch (c) {
            case '0'...'9': {
                float newFloat = strtof(state->readBuf + state->curIdx, &endptr);
                size_t diff = endptr - (state->readBuf + state->curIdx);
                state->curIdx += diff - 1;
                numbers[numNums++] = newFloat;
                break;
            }
            case ' ':
            case '\t':
                break;
            default:    
                endLoop = 1;
                break;
        }
        
        // TODO: finish
        state->curIdx++;
    }

    // get the type
    switch (numNums) {
        case 1:
            return (ms_token){ .type = MS_TT_NUMBER, .value = { .num = numbers[0] } };
        case 2:
            return (ms_token){ .type = MS_TT_VEC2, .value = { .v2 = vec2_new(numbers[0], numbers[1]) } };
        case 3:
            return (ms_token){ .type = MS_TT_VEC3, .value = { .v3 = vec3_new(numbers[0], numbers[1], numbers[2]) } };
        case 4:
            return (ms_token){ .type = MS_TT_VEC4, .value = { .v4 = ui_vec_new(numbers[0], numbers[1], numbers[2], numbers[3]) } };
        default:
            fprintf(stderr, "can't load a numerical value with %d numbers.\n", numNums);
            exit(1);
    }
}

// TODO: use strndup instead.
// tokenize a string; either quote is fine
ms_token _tokenize_string(_lexer_state *state, char startChar) {

    // store the string
    char *strBuf = malloc(sizeof(char) * state->numRead);
    int numChars = 0;

    // get the characters
    state->curIdx++;
    while (state->curIdx < state->numRead && state->readBuf[state->curIdx] != startChar) {
        strBuf[numChars++] = state->readBuf[state->curIdx++];
    }

    // resize the buf to save memory
    char *attemptResize = realloc(strBuf, sizeof(char) * (numChars + 1));
    if (attemptResize != NULL) {
        strBuf = attemptResize;
    }
    strBuf[numChars] = '\0';

    // return the string
    return (ms_token){ .type = MS_TT_STRING, .value = { .chars = strBuf } };
}

// tokenize a whole file
ms_tokens tokenize(const char *filepath) {

    // create the lexer state
    _lexer_state ls = { 0 };

    // load the file
    ls.f = fopen(filepath, "r");
    assert(ls.f != NULL);

    // allocate the buffer for characters
    ls.readBuf = NULL;
    ls.bufSize = 0;
    ls.numRead = 0;

    // create the da
    ms_tokens tokens = ms_tokens_new();

    // read!
    while ((ls.numRead = getline(&ls.readBuf, &ls.bufSize, ls.f)) > 0) {
        
        // iterate over the characters
        ls.curIdx = 0;
        while (ls.curIdx < ls.numRead) {

            // get teh current character
            char c = ls.readBuf[ls.curIdx];

            // skip comments
            if (c == '#')
                break;

            // go through the tokens
            switch (c) {

                // identifiers
                case 'A'...'Z':
                case 'a'...'z':
                case '_':
                    ms_tokens_append(&tokens, _tokenize_identifier(&ls));
                    continue;

                // numbers and all vec types
                case '0'...'9':
                    ms_tokens_append(&tokens, _tokenize_numbers(&ls));
                    continue;

                // end of line
                case '\n':
                    ms_tokens_append(&tokens, (ms_token){ .type = MS_TT_NEWLINE  });
                    break;
                
                // arithmetic
                case '+':
                    ms_tokens_append(&tokens, (ms_token){ .type = MS_TT_PLUS  });
                    break;
                case '-':
                    ms_tokens_append(&tokens, (ms_token){ .type = MS_TT_MINUS  });
                    break;
                case '*':
                    ms_tokens_append(&tokens, (ms_token){ .type = MS_TT_MUL  });
                    break;
                case '/':
                    ms_tokens_append(&tokens, (ms_token){ .type = MS_TT_DIV  });
                    break;

                // strings
                case '"':
                case '\'':
                    ms_tokens_append(&tokens, _tokenize_string(&ls, c));
                    continue;
            }

            ls.curIdx++;
        }

        // free the buffer
    }
    
    // close the file
    fclose(ls.f);
    free(ls.readBuf);

    // return the da
    return tokens;
}