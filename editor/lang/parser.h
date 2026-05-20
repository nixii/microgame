
#ifndef MG_EDITOR_PARSER_H
#define MG_EDITOR_PARSER_H

#include "lexer.h"
#include "../da.h"

// forward declare a node
typedef struct ms_node ms_node;

// all possible node types
typedef enum {
    
    // commands
    MS_NT_CMD_SET,

    // parameters
    MS_NT_PARAM_VALUE,
    MS_NT_PARAM_IDENTIFIER,

    // operators
    MS_NT_OP_PLUS,
    MS_NT_OP_MINUS,
    MS_NT_OP_MUL,
    MS_NT_OP_DIV,
} ms_node_type;

// binary operations
typedef struct {
    ms_node *left;
    ms_node *right;
} ms_node_binop;

// a variable access
typedef struct {
    const char *name;
} ms_node_access;

// all kinds of values
typedef union {
    ms_node_binop binop;
    ms_node_access access;
    ms_token value;
} ms_node_value;

// any kind of node
typedef struct ms_node {
    ms_node_value value;
    ms_node_type type;
} ms_node;

// declare all needed dynamic arrays
DA_DECLARE(ms_nodes, ms_node)
DA_DECLARE(ms_names, char*)

// ast context
typedef struct ms_ast_context {
    struct ms_ast_context *parentContext;
    ms_names variableNames;
    ms_tokens variableValues;
} ms_ast_context;

// a final ast
typedef struct {
    ms_nodes nodes;
    ms_ast_context context;
} ms_ast;

// create an ast
ms_ast parse(ms_tokens tokens);

#endif // MG_EDITOR_PARSER_H