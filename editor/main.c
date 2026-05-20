
#include <stdio.h>
#include "lang/lexer.h"

int main(void) {
    ms_tokens tokens = tokenize("designing_the_engine/scene.microscript");
    printf("%d\n", tokens.length);
    for (int i = 0; i < tokens.length; i++) {
        printf("%d  ", tokens.data[i].type);
        if (tokens.data[i].type == MS_TT_NEWLINE)
            printf("\n");
    }
}