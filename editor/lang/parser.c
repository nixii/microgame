
#include <stdio.h>

#include "parser.h"

// define the node dynamic array
DA_DEFINE(ms_nodes, ms_node)
DA_DEFINE(ms_names, char*)

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

// expect of a type
ms_token ms_ast_expect(ms_ast *ast, ms_tokens *tokens, ms_token_type type) {
    ms_token t = tokens->data[ast->curPos++];
    if (t.type != type) {
        fprintf(stderr, "expected %d, found %d\n", type, t.type);
        exit(1);
    }
    return t;
}

// get a whole block
ms_node *ms_ast_get_next_block(ms_ast *ast, ms_tokens *tokens) {
    
    // TODO: make this work
    ms_token cmd = ms_ast_expect(ast, tokens, MS_TT_KEYWORD);
    cmd = ms_ast_expect(ast, tokens, MS_TT_IDENT);
    cmd = ms_ast_expect(ast, tokens, MS_TT_NUMBER);
    printf("command.\n");
}

// create an entire ast
ms_ast parse(ms_tokens *tokens) {

    // create the empty ast
    ms_ast ast;
    ast.nodes = ms_nodes_new();
    ast.context = ms_ast_context_new(NULL); // no parent
    ast.curPos = 0;

    // basic expect
    while (ast.curPos < tokens->length) {
        ms_ast_get_next_block(&ast, tokens);
        ast.curPos++;
    }

/*
    planning:

    ms_token first = tokens->data[0];
     set:
      L expect(MS_TT_IDENTIFIER)
         L expect_any(MS_TT_VALUE)
           L expect(MS_TT_NEWLINE)
     echo:
      L expect_any(MS_TT_PARAMETER) // MS_TT_VALUE u MS_TT_IDENTIFIER
         L expect(MS_TT_NEWLINE)
*/

    // return the empty ast
    return ast;
}