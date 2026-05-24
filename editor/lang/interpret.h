
#ifndef MG_EDITOR_INTERPRET_H
#define MG_EDITOR_INTERPRET_H

#include <microgame/microgame.h>
#include "parser.h"

/* TODO:
     1. change the parser to load functions itself
     2. load the ast into the interpreter
     3. get some basic code running!
*/

// dynamic array to declare things
DA_DECLARE(ms_names, char*)

// current scope of the interpreter
typedef struct ms_interpreter_scope {
    ms_names varNames;
    // TODO: variable name types
    ms_names funcNames;
    ms_nodes funcNodes;
    struct ms_interpreter_scope *parentScope;
} ms_interpreter_scope;

// current *context* of the interpreter (object and scene)
typedef struct {
    scene *s;
    entity e;
} ms_interpreter_context;

// the entire interpreter and its context
typedef struct {
    ms_interpreter_scope *scope;
    ms_interpreter_context context;

    ms_ast *ast;
} ms_interpreter;

// create an interpreter
ms_interpreter ms_interpreter_from(ms_ast *ast, scene *s, entity e);

#endif // MG_EDITOR_INTERPRET_H