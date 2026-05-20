
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

// create an entire ast
ms_ast parse(ms_tokens tokens) {

    // create the empty ast
    ms_ast ast;
    ast.nodes = ms_nodes_new();
    ast.context = ms_ast_context_new(NULL); // no parent

    // return the empty ast
    return ast;
}