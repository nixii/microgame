
#include <string.h>
#include <stdio.h>

#include "parser.h"

// define the node dynamic array
DA_DEFINE(ms_nodes, ms_node)
DA_DEFINE(ms_names, char*)

// different queries
ms_token_type _ANY_PARAMETER[] = {
    MS_TT_BOOL,
    MS_TT_NIL,
    MS_TT_IDENT,
    MS_TT_NUMBER,
    MS_TT_STRING,
    MS_TT_VEC2,
    MS_TT_VEC3,
    MS_TT_VEC4,
};




//////////////////////
// IMPLEMENTATION

// create a new ast context
ms_ast_context ms_ast_context_new(ms_ast_context *parent) {
    return (ms_ast_context){
        .parentContext = parent,
        .variableNames = ms_names_new(),
        .variableValues = ms_tokens_new()
    };
}

// make an ast node
ms_node *ms_node_new(ms_node_type type, ms_node_value val) {
    ms_node *n = malloc(sizeof(ms_node));
    n->type = type;
    n->value = val;
    return n;
}

// get the next token
static inline ms_token ms_ast_next(ms_ast *ast, ms_tokens *tokens) {
    return tokens->data[ast->curPos++];
}

// expect of a type
ms_token ms_ast_expect(ms_ast *ast, ms_tokens *tokens, ms_token_type type) {
    ms_token t = ms_ast_next(ast, tokens);
    if (t.type != type) {
        fprintf(stderr, "expected token of type %d, found type %d\n", type, t.type);
        exit(1);
    }
    return t;
}

// expect any of these types
ms_token ms_ast_expect_any(ms_ast *ast, ms_tokens *tokens, size_t numTypes, ms_token_type *types) {
    for (size_t i = 0; i < numTypes; i++) {
        ms_token t = tokens->data[ast->curPos];
        if (t.type == types[i]) {
            ast->curPos++;
            return t;
        }
    }
    fprintf(stderr, "expected a token of a group of types, found none.");
    exit(1);
}

static inline ms_token ms_ast_expect_parameter(ms_ast *ast, ms_tokens *tokens) {
    return ms_ast_expect_any(ast, tokens, sizeof(_ANY_PARAMETER) / sizeof(ms_token_type), _ANY_PARAMETER);
}




///////////////////////
// HANDLE COMMANDS
ms_node *ms_ast_parse_echo(ms_ast *ast, ms_tokens *tokens) {}




///////////////////////
// COMMAND HASH

// function handlers for each command type
typedef ms_node *(*_ms_parse_fn)(ms_ast *ast, ms_tokens *tokens);

// store the name and function
typedef struct {
    const char *keyword;
    _ms_parse_fn parse;
} _ms_command_entry;

// the table of commands
static const _ms_command_entry _ms_command_table[] = {
    { "echo", ms_ast_parse_echo }
};
#define MS_COMMAND_COUNT (sizeof(_ms_command_table) / sizeof(_ms_command_table[0]))




///////////////////////
// ACTUALLY PAERSE

// get a whole block
ms_node *ms_ast_get_next_block(ms_ast *ast, ms_tokens *tokens) {
}

// create an entire ast
ms_ast parse(ms_tokens *tokens) {

    // create the empty ast
    ms_ast ast;
    ast.nodes = ms_nodes_new();
    ast.context = ms_ast_context_new(NULL); // no parent
    ast.curPos = 0;

    // basic expect
    while (ast.curPos < (size_t)tokens->length) {
        ms_ast_get_next_block(&ast, tokens);
        ast.curPos++;
    }

    // return the empty ast
    return ast;
}