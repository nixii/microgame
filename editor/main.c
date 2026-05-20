
#include <stdio.h>
#include "lang/lexer.h"
#include "lang/parser.h"

int main(void) {
    ms_tokens tokens = tokenize("designing_the_engine/test.microscript");
    // for (int i = 0; i < tokens.length; i++) {
    //     printf("%d  ", tokens.data[i].type);
    //     if (tokens.data[i].type == MS_TT_NEWLINE)
    //         printf("\n");
    // }
    ms_ast ast = parse(tokens);
}