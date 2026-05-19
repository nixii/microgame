
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

    // go back a char
    state->curIdx--;

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

    // create the new string
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
    char *numStrs[4];
    int endLoop = 0;

    // find the numbers
    while (state->curIdx < state->numRead && !endLoop) {
        // TODO: finish
        state->curIdx++;
    }
    state->curIdx--;
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
        for (ls.curIdx = 0; ls.curIdx < ls.numRead; ls.curIdx++) {

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
                    break;

                // numbers and all vec types
                case '0'...'9':
                    ms_tokens_append(&tokens, _tokenize_numbers(&ls));
                    break;

                // end of line
                case '\n':
                    ms_tokens_append(&tokens, (ms_token){ .type = MS_TT_NEWLINE  });
            }
        }

        // free the buffer
    }
    
    // close the file
    fclose(ls.f);
    free(ls.readBuf);

    // return the da
    return tokens;
}