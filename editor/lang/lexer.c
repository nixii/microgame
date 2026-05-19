
#include <stdio.h>
#include <assert.h>
#include "lexer.h"

// define the da
DA_DEFINE(ms_tokens, ms_token);

// tokenize a string
ms_tokens tokenize(const char *filepath) {

    // load the file
    FILE *f = fopen(filepath, "r");
    assert(f != NULL);

    // allocate the buffer for characters
    char *readBuf = NULL;
    size_t bufSize = 0;
    ssize_t numRead = 0;

    // create the da
    ms_tokens tokens = ms_tokens_new();

    // read!
    while ((numRead = getline(&readBuf, &bufSize, f)) > 0) {
        
        // iterate over the characters
        for (ssize_t i = 0; i < numRead; i++) {

            // get teh current character
            char c = readBuf[i];

            // skip comments
            if (c == '#')
                break;
            printf("%c", c);
        }
    }

    // close the file
    fclose(f);
    free(readBuf);

    // return the da
    return tokens;
}