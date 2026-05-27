
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
ms_node *ms_ast_parse_command_echo(ms_ast *ast, ms_tokens *toks) {

    // create the base command
    ms_node *cmd = ms_node_new(MS_NT_INVOKE, (ms_node_value){ .invoke = {
        .eventName = "echo",
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
    cmd->value.invoke.firstParam = ms_node_new(MS_NT_PARAM, (ms_node_value){ .param = { .data = next, .nextParam = NULL } });

    // set the do if it exists
    ms_token *nextTok = ms_ast_peek(ast, toks);
    if (nextTok != NULL && nextTok->type == MS_TT_KEYWORD && strcmp(nextTok->value.chars, "with") == 0) {
        ms_ast_advance(ast, toks);
        ms_node *block = ms_ast_parse_command_generic_block(ast, toks);
        cmd->value.invoke.firstParam->value.param.nextParam = block;
    }

    // all done!
    return cmd;
}




/////////////////////
// GENERAL DIRECTIVES

// direct to the correct command
ms_node *ms_ast_parse_command(ms_ast *ast, ms_tokens *toks) {
    ms_token *tok = ms_ast_advance(ast, toks);

    // do correct command
    if (strcmp(tok->value.chars, "echo") == 0) {
        return ms_ast_parse_command_echo(ast, toks);
    } else if (strcmp(tok->value.chars, "let") == 0) {
        return ms_ast_parse_command_let(ast, toks, 1);
    } else if (strcmp(tok->value.chars, "set") == 0) {
        return ms_ast_parse_command_let(ast, toks, 0);
    } else if (strcmp(tok->value.chars, "get") == 0) {
        return ms_ast_parse_command_get(ast, toks);
    } else if (strcmp(tok->value.chars, "end") == 0) {
        return ms_ast_parse_command_end(ast, toks);
    } else if (strcmp(tok->value.chars, "on") == 0) {
        return ms_ast_parse_command_on(ast, toks);
    } else if (strcmp(tok->value.chars, "do") == 0) {
        return ms_ast_parse_command_generic_block(ast, toks);
    } else if (strcmp(tok->value.chars, "new") == 0) {
        return ms_ast_parse_command_new(ast, toks);
    }

    return NULL;
}

// parse a value
ms_node *ms_ast_parse_literal(ms_ast *ast, ms_tokens *toks) {
    // FIXME: dereference; good idea?
    return ms_node_new(MS_NT_LITERAL, (ms_node_value){ .literal = *ms_ast_advance(ast, toks) });
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