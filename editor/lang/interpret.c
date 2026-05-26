#include <microgame/microgame.h>

#include "interpret.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "lexer.h"

// names
DA_DEFINE(ms_names, const char*)
DA_DEFINE(ms_datas, ms_data)

// pre-declare
static ms_data ms_interpreter_run_code(ms_interpreter *interp, const ms_node *n);





//////////////
// HELPERS

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
static ms_interpreter_context *ms_interpreter_context_new(scene *s, entity e, ms_interpreter_context *parent) {
    ms_interpreter_context *ctx = malloc(sizeof(ms_interpreter_context));
    ctx->parentContext = parent;
    ctx->s = s;
    ctx->e = e;
    return ctx;
}

// append formatted
static void appendf(char **buf, size_t *len, const char *fmt, ...) {
    va_list args;

    // get the args
    va_start(args, fmt);
    int needed = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    // nothing needed
    if (needed < 0) return;

    // resize it
    *buf = realloc(*buf, *len + needed + 1);

    // append
    va_start(args, fmt);
    vsnprintf(*buf + *len, needed + 1, fmt, args);
    va_end(args);

    // increase size
    *len += needed;
}

// append an ms_data
// TODO: support all datatypes
static void ms_interpreter_appendf_data(char **buf, size_t *len, ms_data d) {
    switch (d.type) {
        case MS_DT_BOOL:
            appendf(buf, len, "%s", d.value.boolean ? "true" : "false");
            break;
        case MS_DT_NIL:
            appendf(buf, len, "nil");
            break;
        case MS_DT_NUMBER:
            appendf(buf, len, "%.3f", d.value.num);
            break;
        case MS_DT_STRING:
            appendf(buf, len, "%s", d.value.str);
            break;
        case MS_DT_VEC2:
            appendf(buf, len, "%.3f %.3f", d.value.v2.x, d.value.v2.y);
            break;
        case MS_DT_VEC3:
            appendf(buf, len, "%.3f %.3f %.3f", d.value.v3.x, d.value.v3.y, d.value.v3.z);
            break;
        case MS_DT_VEC4:
            appendf(buf, len, "%.3f %.3f %.3f %.3f", d.value.v4.scaleX, (float)d.value.v4.pixelsX, d.value.v4.scaleY, (float)d.value.v4.pixelsY);
            break;
        default:
            fprintf(stderr, "ms_interpreter_appendf_data does not support type %d\n", d.type);
            exit(1);
    }
}

// get a variable
static ms_data ms_interpreter_get_variable(ms_interpreter *interp, const char *name) {

    // get the scope
    ms_interpreter_scope *s = interp->scope;

    // iterate through scopes
    while (s != NULL) {
        for (int i = 0; i < s->varNames.length; i++) {
            if (strcmp(s->varNames.data[i], name) == 0) {
                return s->varValues.data[i];
            }
        }
        s = s->parentScope;
    }

    // failure
    fprintf(stderr, "variable %s doesn't exist.\n", name);
    exit(1);
}







///////////////////
// INTERPRETING

// run an event
static ms_data ms_interpreter_load_event(ms_interpreter *interp, const ms_node *n) {
    const char *name = n->value.onCmd.name;
    const ms_node *block = n->value.onCmd.block;
    ms_names_append(&interp->scope->funcNames, name);
    ms_nodes_append(&interp->scope->funcNodes, block);
    printf("do %s loaded.\n", name);
    return (ms_data){ .type = MS_DT_EVENT, .value = (ms_data_value){ .str = name } };
}

// run the ECHO event
static ms_data ms_interpreter_run_code_invoke_echo(ms_interpreter *interp, const ms_node *n) {
    (void)interp;

    // get the parameter
    ms_node *param = n->value.invoke.firstParam;

    // echo is prefixed for now
    printf("echo >  ");
    
    // the string being returned
    char *out = NULL;
    size_t len = 0;

    // while there are parameters
    while (param != NULL) {

        // get the token value
        ms_token *t = &param->value.param.data->value.literal;

        // format-print it
        switch (t->type) {
            case MS_TT_STRING:
                appendf(&out, &len, "%s", t->value.chars);
                break;
            case MS_TT_NUMBER:
                appendf(&out, &len, "%f", t->value.num);
                break;
            case MS_TT_NIL:
                appendf(&out, &len, "nil");
                break;
            case MS_TT_BOOL:
                appendf(&out, &len, t->value.truthy ? "true" : "false");
                break;
            case MS_TT_VEC2:
                appendf(&out, &len, "%.3f %.3f", t->value.v2.x, t->value.v2.y);
                break;
            case MS_TT_VEC3:
                appendf(&out, &len, "%.3f %.3f %.3f", t->value.v3.x, t->value.v3.y, t->value.v3.z);
                break;
            case MS_TT_VEC4:
                appendf(&out, &len, "%.3f %.3f %.3f %.3f", t->value.v4.scaleX, (float) t->value.v4.pixelsX, t->value.v4.scaleY, (float) t->value.v4.pixelsY);
                break;
            case MS_TT_IDENT: {

                // iterate through scopes
                ms_data value = ms_interpreter_get_variable(interp, t->value.chars);

                // switch depending on the type of data
                ms_interpreter_appendf_data(&out, &len, value);
                break;
            }
            default:
                fprintf(stderr, "unsupported value type for echo: %d.\n", param->value.param.data->value.literal.type);
                exit(1);
        }

        // next param
        param = param->value.param.nextParam;
    }
    printf("%s\n", out);
    return (ms_data){ .type = MS_DT_STRING, .value = (ms_data_value){ .str = out } };
}

// make a new instance of type
static ms_data ms_interpreter_spawn_instance(ms_interpreter *interp, const char *typeName) {
    
    if (strcmp(typeName, "entity") == 0) {
        // TODO: spawn an entity
        return (ms_data){ .type = MS_DT_ENTITY, .value = (ms_data_value){ .entity = scene_spawn(interp->context->s) } };
    }

    // failure
    fprintf(stderr, "couldn't create instance of %s\n", typeName);
    exit(1);
}

// new of something
static ms_data ms_interpreter_run_code_invoke_new(ms_interpreter *interp, const ms_node *n) {

    // the first parameter
    ms_node *firstParam = n->value.invoke.firstParam;
    assert(firstParam != NULL);

    // the name of the type
    const char *typeName = firstParam->value.param.data->value.literal.value.chars;

    // instantiate the instance
    ms_data instance = ms_interpreter_spawn_instance(interp, typeName); // TODO: make the instance

    // the  block parameter
    ms_node *withBlock = firstParam->value.param.nextParam;
    if (withBlock != NULL) {

        // set the scope and context

        // call the block

    }

    // return the instance
    return instance;
}

// run invoked code
static ms_data ms_interpreter_run_code_invoke(ms_interpreter *interp, const ms_node *n) {

    // the name of the event
    const char *name = n->value.invoke.eventName;

    // engine functions
    if (strcmp(name, "echo") == 0)
        return ms_interpreter_run_code_invoke_echo(interp, n);
    else if (strcmp(name, "new") == 0)
        return ms_interpreter_run_code_invoke_new(interp, n);

    // error for no event foind
    fprintf(stderr, "event %s not found.\n", n->value.invoke.eventName);
    exit(1);
}

// run the let command
static ms_data ms_interpreter_run_code_cmd_let(ms_interpreter *interp, const ms_node *n) {
    
    // the stuff to use
    const char *name = n->value.letCmd.name;
    const ms_node *internalVal = n->value.letCmd.data;

    // calculate the value of running the value
    ms_data d = ms_interpreter_run_code(interp, internalVal);

    // iterate through all scopes
    ms_interpreter_scope *s = interp->scope;
    while (s != NULL) {
        for (int i = 0; i < s->varNames.length; i++) {
            if (strcmp(s->varNames.data[i], name) == 0) {
                s->varValues.data[i] = d;
                printf("SETting %s.\n", name);
                return d;
            }
        }
        s = s->parentScope;
    }

    // append to this scope
    ms_names_append(&interp->scope->varNames, name);
    ms_datas_append(&interp->scope->varValues, d);

    // just return it back!
    return d;
}

// literal
static ms_data ms_interpreter_run_code_literal(ms_interpreter *interp, const ms_node *n) {
    (void)(interp);
    ms_token tok = n->value.literal;

    // create value
    switch (tok.type) {
        case MS_TT_BOOL:
            return (ms_data){ .type = MS_DT_BOOL, .value = (ms_data_value){ .boolean = tok.value.truthy } };
        case MS_TT_NIL:
            return (ms_data){ .type = MS_DT_NIL };
        case MS_TT_NUMBER:
            return (ms_data){ .type = MS_DT_NUMBER, .value = (ms_data_value){ .num = tok.value.num } };
        case MS_TT_STRING:
            return (ms_data){ .type = MS_DT_STRING, .value = (ms_data_value){ .str = tok.value.chars } };
        case MS_TT_VEC2:
            return (ms_data){ .type = MS_DT_VEC2, .value = (ms_data_value){ .v2 = tok.value.v2 } };
        case MS_TT_VEC3:
            return (ms_data){ .type = MS_DT_VEC3, .value = (ms_data_value){ .v3 = tok.value.v3 } };
        case MS_TT_VEC4:
            return (ms_data){ .type = MS_DT_VEC4, .value = (ms_data_value){ .v4 = tok.value.v4 } };
        default:
            fprintf(stderr, "no way to interpret literal of type %d\n", tok.type);
            exit(1);
    }
}

// run a certain block of code
static ms_data ms_interpreter_run_code(ms_interpreter *interp, const ms_node *n) {

    // run the correct function
    switch (n->type) {
        // call another function
        case MS_NT_INVOKE:
            return ms_interpreter_run_code_invoke(interp, n);
        case MS_NT_CMD_LET:
            return ms_interpreter_run_code_cmd_let(interp, n);
        case MS_NT_LITERAL:
            return ms_interpreter_run_code_literal(interp, n);
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
        .context = ms_interpreter_context_new(s, e, NULL),
        .ast = ast
    };

    // load the functions
    for (int i = 0; i < ast->nodes.length; i++) {
        const ms_node *n = ast->nodes.data[i];

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