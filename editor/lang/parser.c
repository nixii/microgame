
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"

// define the node dynamic array
DA_DEFINE(ms_nodes, const ms_node*)

// forward-declare functions
ms_node *ms_ast_next(ms_ast *ast, ms_tokens *toks);




////////////
// HELPERS

// look at what the next token is
static inline ms_token *ms_ast_peek(ms_ast *ast, ms_tokens *toks) {
    return &(toks->data[ast->curPos]);
}

// consume the next token
static inline ms_token *ms_ast_advance(ms_ast *ast, ms_tokens *toks) {
    return &(toks->data[ast->curPos++]);
}

// make an ast node
static ms_node *ms_node_new(ms_node_type type, ms_node_value val) {
    ms_node *n = malloc(sizeof(ms_node));
    memset(n, 0, sizeof(ms_node));
    n->type = type;
    n->value = val;
    return n;
}




//////////////////////
// PARSE SPECIFIC COMMANDS

// call to 'echo'
ms_node *ms_ast_parse_command_invoke(ms_ast *ast, ms_tokens *toks, const char *name) {

    // create the base command
    ms_node *cmd = ms_node_new(MS_NT_INVOKE, (ms_node_value){ .invoke = {
        .eventName = name,
        .firstParam = NULL,
        .numParams = 0
    }});

    // links to the params
    ms_node **nextParam = &cmd->value.invoke.firstParam;
    ms_node *toAdd;

    // for all params it can get
    while ((toAdd = ms_ast_next(ast, toks)) != NULL) {
        *nextParam = ms_node_new(MS_NT_PARAM, (ms_node_value){ .param = { .data = toAdd, .nextParam = NULL } });
        nextParam = &(*nextParam)->value.param.nextParam;
        cmd->value.invoke.numParams++;
    }

    // finally return the whole command
    return cmd;
}

// call to 'let' for variables
ms_node *ms_ast_parse_command_let(ms_ast *ast, ms_tokens *toks, int doLetCommand) {

    // get the name
    ms_token *name = ms_ast_advance(ast, toks);
    assert(name->type == MS_TT_IDENT); // TODO: make better error handling system

    // next, get the following block
    ms_node *value = ms_ast_next(ast, toks);
    assert(value != NULL);

    // create the command
    ms_node *cmd = ms_node_new(doLetCommand ? MS_NT_CMD_LET : MS_NT_CMD_SET, (ms_node_value){ .letCmd = {
        .name = name->value.chars,
        .data = value
    }});
    return cmd;
}

// 'as' component
ms_node *ms_ast_parse_command_as(ms_ast *ast, ms_tokens *toks) {

    // get the name
    ms_node *nameNode = ms_ast_next(ast, toks); // TODO: finish

    // next, get the following block
    ms_node *value = ms_ast_next(ast, toks);
    assert(value != NULL);

    // create the command
    ms_node *cmd = ms_node_new(MS_NT_CMD_AS, (ms_node_value){ .asCmd = {
        .who = nameNode,
        .block = value
    }});
    return cmd;
}

// call a generic function
ms_node *ms_ast_parse_command_call(ms_ast *ast, ms_tokens *toks) {

    // get the name
    ms_token *name = ms_ast_advance(ast, toks);
    assert(name->type == MS_TT_IDENT);

    // return the result
    return ms_ast_parse_command_invoke(ast, toks, name->value.chars);
}

// a call to 'get' for properties
ms_node *ms_ast_parse_command_get(ms_ast *ast, ms_tokens *toks) {

    // get the name
    ms_token *name = ms_ast_advance(ast, toks);
    assert(name->type == MS_TT_IDENT);

    // return the final node
    ms_node *cmd = ms_node_new(MS_NT_CMD_GET, (ms_node_value){ .getCmd = { .name = name->value.chars } });
    return cmd;
}

// end a block of code
ms_node *ms_ast_parse_command_end(ms_ast *ast, ms_tokens *toks) {
    (void)(ast); (void)(toks);
    return ms_node_new(MS_NT_CMD_END, (ms_node_value){ 0 });
}

// start a command definition
ms_node *ms_ast_parse_command_on(ms_ast *ast, ms_tokens *toks) {

    // get the name of the function
    ms_token *name = ms_ast_advance(ast, toks);
    assert(name->type == MS_TT_IDENT);

    // get the code of the function
    ms_node *block = ms_ast_next(ast, toks);
    assert(block->type == MS_NT_CMD_DO);

    // return the final definition
    return ms_node_new(MS_NT_CMD_ON, (ms_node_value){ .onCmd = { .name = name->value.chars, .block = block } });
}

// define a block
ms_node *ms_ast_parse_command_generic_block(ms_ast *ast, ms_tokens *toks) {

    // create the block
    ms_node *block = ms_node_new(MS_NT_CMD_DO, (ms_node_value){ .doCmd = { .nodes = ms_nodes_new() } });

    // easy link to all the nodes
    ms_nodes *nodes = &block->value.doCmd.nodes;

    // the next read node
    ms_token *nextTok = NULL;
    
    // while there is a next node
    while ((nextTok = ms_ast_peek(ast, toks)) != NULL && nextTok->type != MS_TT_EOF) {
        if (nextTok->type == MS_TT_NEWLINE) {
            ms_ast_advance(ast, toks);
            continue;
        }

        ms_node *nextNode = ms_ast_next(ast, toks);
        if (nextNode->type == MS_NT_CMD_END) {
            free(nextNode);
            break;
        }

        ms_nodes_append(nodes, nextNode);
    }

    // return the block
    return block;
}

// create a new instance of something
ms_node *ms_ast_parse_command_new(ms_ast *ast, ms_tokens *toks) {

    // create the command
    ms_node *cmd = ms_node_new(MS_NT_INVOKE, (ms_node_value){ .invoke = {
        .eventName = "new",
        .firstParam = NULL,
        .numParams = 0
    }});

    // get the type to go to
    ms_node *next = ms_ast_next(ast, toks);
    assert(next != NULL);

    // set the type
    ms_node *lastParam = ms_node_new(MS_NT_PARAM, (ms_node_value){ .param = { .data = next, .nextParam = NULL } });
    cmd->value.invoke.firstParam = lastParam;
    cmd->value.invoke.numParams++;

    // is there a next section with "from"?
    ms_token *nextTok = ms_ast_peek(ast, toks);
    if (nextTok != NULL && nextTok->type == MS_TT_KEYWORD && strcmp(nextTok->value.chars, "from") == 0) {
        ms_ast_advance(ast, toks);
        lastParam->value.param.nextParam = ms_node_new(MS_NT_PARAM, (ms_node_value){
            .param = { .data = ms_ast_next(ast, toks), .nextParam = NULL }
        });
        lastParam = lastParam->value.param.nextParam;
        cmd->value.invoke.numParams++;
    }

    // with block section
    nextTok = ms_ast_peek(ast, toks);
    if (nextTok != NULL && nextTok->type == MS_TT_KEYWORD && strcmp(nextTok->value.chars, "with") == 0) {
        ms_ast_advance(ast, toks);
        lastParam->value.param.nextParam = ms_node_new(MS_NT_PARAM, (ms_node_value){
            .param = { .data = ms_ast_parse_command_generic_block(ast, toks), .nextParam = NULL }
        });
        lastParam = lastParam->value.param.nextParam;
        cmd->value.invoke.numParams++;
    }

    // all done!
    return cmd;
}

// attach command
ms_node *ms_ast_parse_command_attach(ms_ast *ast, ms_tokens *toks) {
    
    // get the next thing
    ms_node *next = ms_ast_next(ast, toks);
    assert(next != NULL);

    // create the command
    ms_node *command = ms_node_new(
        MS_NT_INVOKE, 
        (ms_node_value){
            .invoke = {
                .eventName = "attach", .numParams = 1, 
                .firstParam = ms_node_new(MS_NT_PARAM, (ms_node_value){ .param = { .nextParam = NULL, .data = next } })
            } 
        });

    // done!
    return command;
}




/////////////////////
// GENERAL DIRECTIVES

// direct to the correct command
ms_node *ms_ast_parse_command(ms_ast *ast, ms_tokens *toks) {
    ms_token *tok = ms_ast_advance(ast, toks);

    // do correct command
    if (strcmp(tok->value.chars, "echo") == 0) {
        return ms_ast_parse_command_invoke(ast, toks, "echo");
    } else if (strcmp(tok->value.chars, "load_mesh") == 0) {
        return ms_ast_parse_command_invoke(ast, toks, "load_mesh");
    } else if (strcmp(tok->value.chars, "let") == 0) {
        return ms_ast_parse_command_let(ast, toks, 1);
    } else if (strcmp(tok->value.chars, "as") == 0) {
        return ms_ast_parse_command_as(ast, toks);
    } else if (strcmp(tok->value.chars, "set") == 0) {
        return ms_ast_parse_command_let(ast, toks, 0);
    } else if (strcmp(tok->value.chars, "get") == 0) {
        return ms_ast_parse_command_get(ast, toks);
    } else if (strcmp(tok->value.chars, "attach") == 0) {
        return ms_ast_parse_command_attach(ast, toks);
    } else if (strcmp(tok->value.chars, "end") == 0) {
        return ms_ast_parse_command_end(ast, toks);
    } else if (strcmp(tok->value.chars, "on") == 0) {
        return ms_ast_parse_command_on(ast, toks);
    } else if (strcmp(tok->value.chars, "do") == 0) {
        return ms_ast_parse_command_generic_block(ast, toks);
    } else if (strcmp(tok->value.chars, "new") == 0) {
        return ms_ast_parse_command_new(ast, toks);
    } else if (strcmp(tok->value.chars, "call") == 0) {
        return ms_ast_parse_command_call(ast, toks);
    }

    return NULL;
}



// Operation order prescedence
static int ms_binop_precedence(ms_token_type t) {
    switch (t) {
        case MS_TT_LESS_THAN:
        case MS_TT_GREATER_THAN:
        case MS_TT_EQUALS:
            return 1;
        case MS_TT_PLUS:
        case MS_TT_MINUS:
            return 2;
        case MS_TT_MULTIPLY:
        case MS_TT_DIVIDE:
            return 3;
        default:
            return -1;
    }
}

static ms_node *ms_ast_parse_expression(ms_ast *ast, ms_tokens *toks, int minimumPrescedence);

// parse a lit or parenthesized expr
static ms_node *ms_ast_parse_primary(ms_ast *ast, ms_tokens *toks) {
    ms_token *tok = ms_ast_peek(ast, toks);

    if (tok && tok->type == MS_TT_LPAREN) {
        ms_ast_advance(ast, toks);
        ms_node *inner = ms_ast_parse_expression(ast, toks, 0);
        ms_ast_advance(ast, toks);
        return inner;
    }

    return ms_node_new(MS_NT_LITERAL, (ms_node_value){ .literal = *ms_ast_advance(ast, toks) });
}

// parse an expression
static ms_node *ms_ast_parse_expression(ms_ast *ast, ms_tokens *toks, int minimumPrescedence) {
    ms_node *left = ms_ast_parse_primary(ast, toks);

    while (1) {
        ms_token *next = ms_ast_peek(ast, toks);
        if (next == NULL) break;

        int prec = ms_binop_precedence(next->type);
        if (prec < minimumPrescedence) break;

        ms_token_type op = ms_ast_advance(ast, toks)->type;
        ms_node *right = ms_ast_parse_expression(ast, toks, prec + 1);

        left = ms_node_new(
            MS_NT_BINOP,
            (ms_node_value){
                .binOp = {
                    .a = left,
                    .b = right,
                    .operation = op
                }
            });
    }

    return left;
}

// parse a value
ms_node *ms_ast_parse_literal(ms_ast *ast, ms_tokens *toks) {
    return ms_ast_parse_expression(ast, toks, 0);
}



////////////////
// NEXT LINE
ms_node *ms_ast_next(ms_ast *ast, ms_tokens *toks) {
    ms_token *tok = ms_ast_peek(ast, toks);
    assert(tok != NULL);

    // parse commands
    switch (tok->type) {

        // parse a new command block
        case MS_TT_KEYWORD:
            return ms_ast_parse_command(ast, toks);

        // return nothing on a newline
        case MS_TT_NEWLINE:
            break;
        
        // end the file
        case MS_TT_EOF:
            break;
        
        // parse an expression
        default:
            return ms_ast_parse_literal(ast, toks);
    }

    // nothing worked
    ms_ast_advance(ast, toks);
    return NULL;
}




// create an entire ast
ms_ast parse(ms_tokens *tokens) {
    printf("parser started\n");

    // create the empty ast
    ms_ast ast;
    ast.nodes = ms_nodes_new();
    ast.curPos = 0;

    // basic expect
    while (ast.curPos < (size_t)tokens->length) {
        ms_node *new = ms_ast_next(&ast, tokens);
        if (new != NULL)
            ms_nodes_append(&ast.nodes, new);
    }

    // return the empty ast
    return ast;
}