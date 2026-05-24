
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
DA_DECLARE(ms_names, const char*)

// datatypes
typedef enum {

    // primitives
    MS_DT_STRING,
    MS_DT_NUMBER,
    MS_DT_BOOL,
    MS_DT_NIL,

    // math
    MS_DT_VEC2,
    MS_DT_VEC3,
    MS_DT_VEC4,

    // special types
    MS_DT_EVENT,
    
    // engine objects
    MS_DT_SCENE,
    MS_DT_ENTITY
} ms_data_type;

// a ms value
typedef union {
    const char *str;
    float num;
    int boolean;

    vec2 v2;
    vec3 v3;
    ui_vec v4;
    
    scene *s;
    entity e;
} ms_data_value;

// a data point
typedef struct {
    ms_data_type type;
    ms_data_value value;
} ms_data;

// declare the data dyn arr
DA_DECLARE(ms_datas, ms_data);

// current scope of the interpreter
typedef struct ms_interpreter_scope {
    ms_names varNames;
    ms_datas varValues;
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