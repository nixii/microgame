
#ifndef MG_EDITOR_INTERPRET_H
#define MG_EDITOR_INTERPRET_H

#include <microgame/microgame.h>
#include "parser.h"
#include "data.h"

/* TODO:
     1. change the parser to load functions itself
     2. load the ast into the interpreter
     3. get some basic code running!
*/

// dynamic array to declare things
DA_DECLARE(ms_names, const char*)

// current scope of the interpreter
typedef struct ms_interpreter_scope {
    ms_names varNames;
    ms_datas varValues;
    ms_names funcNames;
    ms_nodes funcNodes;
    ms_nodes funcParams;
    struct ms_interpreter_scope *parentScope;
} ms_interpreter_scope;

// current *context* of the interpreter (object and scene)
typedef struct ms_interpreter_context {
    scene *s;
    entity e;
    ms_data obj;
    struct ms_interpreter_context *parentContext;
} ms_interpreter_context;

// the entire interpreter and its context
typedef struct {
    ms_interpreter_scope *scope;
    ms_interpreter_context *context;

    ms_ast *ast;
} ms_interpreter;

// create an interpreter
ms_interpreter ms_interpreter_from(ms_ast *ast, scene *s, entity e, ms_data obj);
ms_data ms_data_nil();
void ms_interpreter_call_all_frame_fns(ms_interpreter *interp);

#endif // MG_EDITOR_INTERPRET_H