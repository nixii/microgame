#include <microgame/microgame.h>

#include "interpret.h"
#include <string.h>
#include "lexer.h"
#include <stdio.h>

// names
DA_DEFINE(ms_names, char*)

// create a new scope with a possible parent
// NULL is a fine parameter
static ms_interpreter_scope *ms_interpreter_scope_new(ms_interpreter_scope *parent) {
    ms_interpreter_scope *s = malloc(sizeof(ms_interpreter_scope));
    s->parentScope = parent;
    s->varNames = ms_names_new();
    s->funcNames = ms_names_new();
    s->funcNodes = ms_nodes_new();
    return s;
}

// create a new context window with the entity and scene
static ms_interpreter_context ms_interpreter_context_new(scene *s, entity e) {
    return (ms_interpreter_context){
        .s = s,
        .e = e
    };
}

// run an event
static void ms_interpreter_load_event(ms_interpreter *interp, ms_node *n) {
    const char *name = n->value.onCmd.name;
    const ms_node *block = n->value.onCmd.block;
    ms_names_append(&interp->scope->funcNames, name);
    ms_nodes_append(&interp->scope->funcNodes, block);
    printf("do %s loaded.\n", name);
}

// run the ECHO event
static void ms_interpreter_run_code_invoke_echo(ms_interpreter *interp, ms_node *n) {

    // get the parameter
    ms_node *param = n->value.invoke.firstParam;

    // echo is prefixed for now
    printf("echo >  ");

    // while there are parameters
    while (param != NULL) {

        // get the token value
        ms_token *t = &param->value.param.value->value.literal;

        // format-print it
        switch (t->type) {
            case MS_TT_STRING:
                printf("%s", t->value.chars);
                break;
            case MS_TT_NUMBER:
                printf("%f", t->value.num);
                break;
            case MS_TT_NIL:
                printf("nil");
                break;
            case MS_TT_BOOL:
                printf(t->value.truthy ? "true" : "false");
                break;
            case MS_TT_VEC2:
                printf("%.3f %.3f", t->value.v2.x, t->value.v2.y);
                break;
            case MS_TT_VEC3:
                printf("%.3f %.3f %.3f", t->value.v3.x, t->value.v3.y, t->value.v3.z);
                break;
            case MS_TT_VEC4:
                printf("%.3f %.3d %.3f %3d", t->value.v4.scaleX, t->value.v4.pixelsX, t->value.v4.scaleY, t->value.v4.pixelsY);
                break;
            default:
                fprintf(stderr, "unsupported type. %d\n", param->value.param.value->value.literal.type);
                exit(1);
        }

        // next param
        param = param->value.param.nextParam;
    }
    // printf("ECHO :: %s\n", n->value.invoke.firstParam->value.param.value->value.literal.value.chars);
    printf("\n");
}

// run invoked code
static void ms_interpreter_run_code_invoke(ms_interpreter *interp, ms_node *n) {

    // the name of the event
    const char *name = n->value.invoke.eventName;

    if (strcmp(name, "echo") == 0)
        return ms_interpreter_run_code_invoke_echo(interp, n);

    // error for no event foind
    fprintf(stderr, "event %s not found.\n", n->value.invoke.eventName);
    exit(1);
}

// run a certain block of code
static void ms_interpreter_run_code(ms_interpreter *interp, ms_node *n) {

    // run the correct function
    switch (n->type) {
        // call another function
        case MS_NT_INVOKE:
            return ms_interpreter_run_code_invoke(interp, n);
        default:
            break;
    }

    // failure case
    fprintf(stderr, "no way to run type %d\n", n->type);
    exit(1);
}

// create an interpreter
ms_interpreter ms_interpreter_from(ms_ast *ast, scene *s, entity e) {

    // create the interpreter
    ms_interpreter interp = (ms_interpreter){
        .scope = ms_interpreter_scope_new(NULL),
        .context = ms_interpreter_context_new(s, e),
        .ast = ast
    };

    // load the functions
    for (int i = 0; i < ast->nodes.length; i++) {
        ms_node *n = ast->nodes.data[i];

        // update based on the type
        switch (n->type) {
            case MS_NT_CMD_ON:
                ms_interpreter_load_event(&interp, n);
                break;
            default:
                ms_interpreter_run_code(&interp, n);
                break;
        }
    }

    // return the interpreter
    return interp;
}