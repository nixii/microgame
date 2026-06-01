
#ifndef MG_EDITOR_LEXER_H
#define MG_EDITOR_LEXER_H

#include <microgame/microgame.h>
#include "../da.h"

// the amount of chars to read in a line
#define MS_LEXER_LINE_READ_SIZE 4096

// token types
typedef enum {

    // various important things
    MS_TT_IDENT,
    MS_TT_KEYWORD,
    MS_TT_NUMBER,
    MS_TT_VEC2,
    MS_TT_VEC3,
    MS_TT_VEC4,
    MS_TT_STRING,
    MS_TT_BOOL,
    MS_TT_NIL,
    MS_TT_NEWLINE,
    MS_TT_EOF,

    // Operators
    MS_TT_LESS_THAN,
    MS_TT_GREATER_THAN,
    MS_TT_EQUALS,
    MS_TT_PLUS,
    MS_TT_MINUS,
    MS_TT_MULTIPLY,
    MS_TT_DIVIDE,
    MS_TT_LPAREN,
    MS_TT_RPAREN
} ms_token_type;

// token values
typedef union {
    char *chars;
    float num;
    int truthy;
    vec2 v2;
    vec3 v3;
    ui_vec v4;
} ms_token_value;

// actual token object
typedef struct {
    ms_token_type type;
    ms_token_value value;
} ms_token;

// create the dynamic array type
DA_DECLARE(ms_tokens, ms_token);

// load tokens
ms_tokens tokenize(const char *src);

#endif // MG_EDITOR_LEXER_H