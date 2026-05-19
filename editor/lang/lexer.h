
#ifndef MG_EDITOR_LEXER_H
#define MG_EDITOR_LEXER_H

typedef enum {

    // various important things
    MS_TT_IDENT,
    MS_TT_NUMBER,
    MS_TT_VEC2,
    MS_TT_VEC3,
    MS_TT_VEC4,
    MS_TT_STRING,
    MS_TT_BOOL,
    MS_TT_NIL,
    MS_TT_NEWLINE,
    MS_TT_EOF,

    // operators
    MS_TT_PLUS,
    MS_TT_MINUS,
    MS_TT_MUL,
    MS_TT_DIV,
} ms_token_type;

// load tokens


#endif // MG_EDITOR_LEXER_H