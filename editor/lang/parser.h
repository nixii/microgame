
#ifndef MG_EDITOR_PARSER_H
#define MG_EDITOR_PARSER_H

#include "lexer.h"
#include "../da.h"

// forward declare a node
typedef struct ms_node ms_node;

// declare all needed dynamic arrays
DA_DECLARE(ms_nodes, const ms_node*)

// all possible node types
typedef enum {
    
    // commands
    MS_NT_CMD_LET,
    MS_NT_CMD_END,
    MS_NT_CMD_ON,
    MS_NT_CMD_DO,

    // parameters and functions
    MS_NT_INVOKE,
    MS_NT_PARAM,
    MS_NT_NEWLINE,

    // literal values
    MS_NT_LITERAL,
} ms_node_type;

// calling a function
typedef struct {
    const char *eventName;
    ms_node *firstParam;
    uint8_t numParams;
} ms_node_invoke;

// a parameter node
typedef struct {
    ms_node *data;
    ms_node *nextParam;
} ms_node_param;

// a set command
typedef struct {
    const char *name;
    ms_node *data;
} ms_node_let;

// a block of code
typedef struct {
    ms_nodes nodes;
} ms_node_do;

// defining a function
typedef struct {
    const char *name;
    ms_node *block;
} ms_node_on;

// all kinds of values
typedef union {
    ms_node_invoke invoke;
    ms_node_param param;
    ms_node_let letCmd;
    ms_node_do doCmd;
    ms_node_on onCmd;
    ms_token literal;
} ms_node_value;

// any kind of node
typedef struct ms_node {
    ms_node_value value;
    ms_node_type type;
} ms_node;

// a final ast
typedef struct {
    ms_nodes nodes;
    size_t curPos;
} ms_ast;

// create an ast
ms_ast parse(ms_tokens *tokens);

#endif // MG_EDITOR_PARSER_H