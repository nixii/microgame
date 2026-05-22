
#include <string.h>
#include <stdio.h>

#include "parser.h"

// define the node dynamic array
DA_DEFINE(ms_nodes, ms_node*)
DA_DEFINE(ms_names, char*)




////////////
// HELPERS
static inline ms_token *ms_ast_peek(ms_ast *ast, ms_tokens *toks) {
    return &(toks->data[ast->curPos]);
}
static inline ms_token *ms_ast_advance(ms_ast *ast, ms_tokens *toks) {
    return &(toks->data[ast->curPos++]);
}




//////////////////////
// HELPERS

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




/////////////////////
// parse a command
ms_node *ms_ast_parse_command(ms_ast *ast, ms_tokens *toks) {
    ms_token *tok = ms_ast_advance(ast, toks);

    // do correct command
    if (strcmp(tok->value.chars, "echo") == 0) {
        printf("echo found!");
    }

    return NULL;
}




////////////////
// NEXT LINE
ms_node *ms_ast_next(ms_ast *ast, ms_tokens *toks) {
    ms_token *tok = ms_ast_peek(ast, toks);

    // parse commands
    switch (tok->type) {

        // parse a new command block
        case MS_TT_KEYWORD:
            return ms_ast_parse_command(ast, toks);
            break;
        
        // parse an expression
        default:
            break;
    }

    // nothing worked
    return NULL;
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
        ms_nodes_append(&ast.nodes, ms_ast_next(&ast, tokens));
        ast.curPos++;
    }

    // return the empty ast
    return ast;
}