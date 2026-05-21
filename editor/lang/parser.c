
#include <string.h>
#include <stdio.h>

#include "parser.h"

// define the node dynamic array
DA_DEFINE(ms_nodes, ms_node)
DA_DEFINE(ms_names, char*)

// length
#define MS_LEN(a) (sizeof(a) / sizeof((a)[0]))

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

// get the next of a type
static inline ms_token *ms_ast_next_of(ms_ast *ast, ms_tokens *tokens, ms_token_type type) {
    if (tokens->data[ast->curPos].type == type)
        return &(tokens->data[ast->curPos++]);
    return NULL;
}
static inline ms_token *ms_ast_next_of_any(ms_ast *ast, ms_tokens *tokens, size_t numTypes, ms_token_type *types) {
    if (ast->curPos >= (size_t)tokens->length) return NULL;
    ms_token *next = &(tokens->data[ast->curPos]);
    for (size_t i = 0; i < numTypes; i++) {
        if (next->type == types[i]) {
            ast->curPos++;
            return next;
        }
    }
    return NULL;
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



///////////////////////
// HANDLE COMMANDS
ms_node *ms_ast_parse_echo(ms_ast *ast, ms_tokens *tokens) {

    // create the root command node
    ms_node *cmd = ms_node_new(MS_NT_CALL, (ms_node_value){ .call = { .funcName = "echo", .firstParam = NULL } });

    // load all the parameters
    ms_node **next = &cmd->value.call.firstParam;
    ms_token *paramToken;
    while ((paramToken = ms_ast_next_of_any(ast, tokens, MS_LEN(_ANY_PARAMETER), _ANY_PARAMETER)) != NULL) {
        *next = ms_node_new(MS_NT_PARAM, (ms_node_value){ .param = { .tok = *paramToken, .nextParam = NULL } });
        next = &(*next)->value.param.nextParam;
    }

    // return the final command
    return cmd;
}




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
    ms_token cmd = ms_ast_expect(ast, tokens, MS_TT_KEYWORD);
    for (size_t i = 0; i < MS_COMMAND_COUNT; i++) {
        if (strcmp(cmd.value.chars, _ms_command_table[i].keyword) == 0)
            return _ms_command_table[i].parse(ast, tokens);
    }
    fprintf(stderr, "command %s not found.\n", cmd.value.chars);
    exit(1);
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