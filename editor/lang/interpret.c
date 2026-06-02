#include <microgame/microgame.h>

#include "properties/entity.h"
#include "properties/scene.h"
#include "properties/collider.h"
#include "properties/velocity.h"
#include "properties/mesh.h"
#include "properties/vec3.h"

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

// create an empty data
ms_data ms_data_nil() {
    return (ms_data){ .type = MS_DT_NIL, .ptr = FALSE };
}






///////////////
// SCOPE

// create a new scope with a possible parent
// NULL is a fine parameter
static ms_interpreter_scope *ms_interpreter_scope_new(ms_interpreter_scope *parent) {
    ms_interpreter_scope *s = calloc(1, sizeof(ms_interpreter_scope));
    s->parentScope = parent;
    s->varNames = ms_names_new();
    s->varValues = ms_datas_new();
    s->funcNames = ms_names_new();
    s->funcNodes = ms_nodes_new();
    s->funcParams = ms_nodes_new();
    return s;
}

// go into a new scope with the given scene and entity
static void ms_interpreter_scope_push(ms_interpreter *interpreter) {
    ms_interpreter_scope *new = ms_interpreter_scope_new(interpreter->scope);
    interpreter->scope = new;
}

// go back up a scope
static void ms_interpreter_scope_pop(ms_interpreter *interpreter) {
    ms_interpreter_scope *parent = interpreter->scope->parentScope;
    ms_names_destroy(&interpreter->scope->varNames);
    ms_datas_destroy(&interpreter->scope->varValues);
    ms_names_destroy(&interpreter->scope->funcNames);
    ms_nodes_destroy(&interpreter->scope->funcNodes);
    ms_nodes_destroy(&interpreter->scope->funcParams);
    free(interpreter->scope);
    interpreter->scope = parent;
}






/////////////
// CONTEXT

// create a new context window with the entity and scene
static ms_interpreter_context *ms_interpreter_context_new(scene *s, entity e, ms_data obj, ms_interpreter_context *parent) {
    ms_interpreter_context *ctx = calloc(1, sizeof(ms_interpreter_context));
    ctx->parentContext = parent;
    ctx->s = s;
    ctx->e = e;
    ctx->obj = obj;

    // special stuff
    if (obj.type == MS_DT_NIL) {
        if (e != NIL_ENTITY)
            ctx->obj = (ms_data){ .type = MS_DT_ENTITY, .ptr = FALSE, .value = { .entity = e } };
        else if (s != NULL)
            ctx->obj = (ms_data){ .type = MS_DT_SCENE, .ptr = TRUE, .value = { .scene = s } };
    }
    return ctx;
}

// push a context
static void ms_interpreter_context_push(ms_interpreter *interp, scene *s, entity e, ms_data obj) {
    ms_interpreter_context *new = ms_interpreter_context_new(s, e, obj, interp->context);
    interp->context = new;
}

// pop a context
static void ms_interpreter_context_pop(ms_interpreter *interp) {
    ms_interpreter_context *parent = interp->context->parentContext;
    free(interp->context);
    interp->context = parent;
}




//////////////
// HELPERS

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
    char *temp = realloc(*buf, *len + needed + 1);
    if (temp == NULL) {
        fprintf(stderr, "temp is null.\n");
        free(*buf);
        *buf = NULL;
        *len = 0;
        exit(1);
    }
    *buf = temp;

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
static ms_data ms_interpreter_run_code_cmd_on(ms_interpreter *interp, const ms_node *n) {
    const char *name = n->value.onCmd.name;
    const ms_node *block = n->value.onCmd.block;
    const ms_node *firstParam = n->value.onCmd.paramDefs;
    ms_names_append(&interp->scope->funcNames, name);
    ms_nodes_append(&interp->scope->funcNodes, block);
    ms_nodes_append(&interp->scope->funcParams, firstParam);
    return (ms_data){ .type = MS_DT_EVENT, .ptr = TRUE, .value = (ms_data_value){ .str = name } };
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
        ms_data res = ms_interpreter_run_code(interp, param->value.param.data);

        // format-print it
        switch (res.type) {
            case MS_DT_STRING:
                appendf(&out, &len, "%s", res.value.str);
                break;
            case MS_DT_NUMBER:
                appendf(&out, &len, "%f", res.value.num);
                break;
            case MS_DT_NIL:
                appendf(&out, &len, "nil");
                break;
            case MS_DT_BOOL:
                appendf(&out, &len, res.value.boolean ? "true" : "false");
                break;
            case MS_DT_VEC2:
                appendf(&out, &len, "%.3f %.3f", res.value.v2.x, res.value.v2.y);
                break;
            case MS_DT_VEC3:
                appendf(&out, &len, "%.3f %.3f %.3f", res.value.v3.x, res.value.v3.y, res.value.v3.z);
                break;
            case MS_DT_VEC4:
                appendf(&out, &len, "%.3f %.3f %.3f %.3f", res.value.v4.scaleX, (float) res.value.v4.pixelsX, res.value.v4.scaleY, (float) res.value.v4.pixelsY);
                break;
            // case MS_T_IDENT: {

            //     // iterate through scopes
            //     ms_data value = ms_interpreter_get_variable(interp, t->value.chars);

            //     // switch depending on the type of data
            //     ms_interpreter_appendf_data(&out, &len, value);
            //     break;
            // }
            default:
                fprintf(stderr, "unsupported value type for echo: %d.\n", res.type);
                exit(1);
        }

        // next param
        param = param->value.param.nextParam;
    }
    printf("%s\n", out);
    return (ms_data){ .type = MS_DT_STRING, .ptr = TRUE, .value = (ms_data_value){ .str = out } };
}

// load a mesh
static ms_data ms_interpreter_run_code_invoke_load_mesh(ms_interpreter *interp, const ms_node *n) {
    
    // get the path
    ms_data meshPath = ms_interpreter_run_code(interp, n->value.invoke.firstParam->value.param.data);

    // create the mesh
    ms_data mesh = (ms_data){ 
        .type = MS_DT_RESOURCE_MESH, 
        .ptr = FALSE, 
        .value = (ms_data_value){ 
            .meshResource = mesh_resource_from_obj(meshPath.value.str)
        }
    };

    // done!
    return mesh;
}

// make a new instance of type
static ms_data ms_interpreter_spawn_instance(ms_interpreter *interp, const char *typeName, ms_data from) {
    
    // TODO: font_resource, image_resource, sound_resource
    if (strcmp(typeName, "entity") == 0) {
        return (ms_data){ .type = MS_DT_ENTITY, .ptr = FALSE, .value = (ms_data_value){ .entity = scene_spawn(interp->context->s) } };

    } else if (strcmp(typeName, "scene") == 0) {
        return (ms_data){ .type = MS_DT_SCENE, .ptr = TRUE, .value = (ms_data_value){ .scene = scene_new() } };

    } else if (strcmp(typeName, "collider") == 0) {
        // DEFAULT SIZE: <1 1 1>
        return (ms_data){ .type = MS_DT_COMPONENT_COLLIDER, .ptr = FALSE, .value = (ms_data_value){ .collider = collider_new(vec3_new(1, 1, 1)) } };
    }

    else if (strcmp(typeName, "velocity") == 0) {
        // DEFAULT VELOCITY: <0 0 0>
        return (ms_data){ .type = MS_DT_COMPONENT_VELOCITY, .ptr = FALSE, .value = (ms_data_value){ .velocity = velocity_new(vec3_zero()) } };
    }

    // things with a "from" keyword
    else if (strcmp(typeName, "mesh") == 0) {
        assert(from.type == MS_DT_RESOURCE_MESH);
        return (ms_data){ .type = MS_DT_COMPONENT_MESH, .ptr = FALSE, .value = (ms_data_value){ .mesh = mesh_from_resource(rgb(255, 255, 255), from.value.meshResource) } };
    }

    // failure
    fprintf(stderr, "couldn't create instance of %s;  doesn't exist\n", typeName);
    exit(1);
}

// new of something
static ms_data ms_interpreter_run_code_invoke_new(ms_interpreter *interp, const ms_node *n) {
    printf("new.\n");
    
    // the first parameter
    ms_node *firstParam = n->value.invoke.firstParam;
    assert(firstParam != NULL);

    // the name of the type
    ms_data typeName = ms_interpreter_run_code(interp, firstParam->value.param.data);
    assert(typeName.type == MS_DT_STRING);

    // is there a from?
    ms_data from = ms_data_nil();
    if (firstParam->value.param.nextParam != NULL &&
        firstParam->value.param.nextParam->value.param.data->type != MS_NT_CMD_DO) {
        from = ms_interpreter_run_code(interp, firstParam->value.param.nextParam->value.param.data);
        firstParam = firstParam->value.param.nextParam;
    }

    // instantiate the instance
    ms_data instance = ms_interpreter_spawn_instance(interp, typeName.value.str, from);

    // is there a with?
    if (firstParam->value.param.nextParam != NULL) {
        ms_interpreter_context_push(
            interp, 
            interp->context->s, 
            instance.type == MS_DT_ENTITY ? instance.value.entity : interp->context->e, 
            instance);
        ms_interpreter_run_code(interp, firstParam->value.param.nextParam->value.param.data);
        instance = interp->context->obj;
        ms_interpreter_context_pop(interp);
    }

    // return the instance
    return instance;
}

// attach something
static ms_data ms_interpreter_run_code_invoke_attach(ms_interpreter *interp, const ms_node *n) {

    // the first parameter
    ms_node *firstParam = n->value.invoke.firstParam;
    assert(firstParam != NULL);

    // the instance
    ms_data value = ms_interpreter_run_code(interp, firstParam->value.param.data);
    printf("attaching type %d\n", value.type);

    // the  block parameter
    return ms_interpreter_entity_attach_component(interp->context->s, interp->context->e, value);
}

// run invoked code
static ms_data ms_interpreter_run_code_invoke(ms_interpreter *interp, const ms_node *n) {

    // the name of the event
    const char *name = n->value.invoke.eventName;

    // engine functions
    if (strcmp(name, "echo") == 0)
        return ms_interpreter_run_code_invoke_echo(interp, n);
    if (strcmp(name, "new") == 0)
        return ms_interpreter_run_code_invoke_new(interp, n);
    if (strcmp(name, "attach") == 0)
        return ms_interpreter_run_code_invoke_attach(interp, n);
    if (strcmp(name, "load_mesh") == 0)
        return ms_interpreter_run_code_invoke_load_mesh(interp, n);

    // run the special event
    // the scope
    ms_interpreter_scope *s = interp->scope;
    while (s != NULL) {
        for (int i = 0; i < s->funcNames.length; i++) {
            if (strcmp(s->funcNames.data[i], name) == 0) {
                
                // push the new scope
                const ms_node *param = s->funcParams.data[i];
                const ms_node *funcNode = s->funcNodes.data[i];
                ms_interpreter_scope_push(interp);
                const ms_node *paramValue = n->value.invoke.firstParam;
                printf("got to loop\n");
                while (param != NULL) {
                    printf("to loop.\n");
                    ms_names_append(&s->varNames, param->value.paramDef.name);
                    if (paramValue == NULL) {
                        ms_datas_append(&s->varValues, ms_data_nil());
                        printf("null.\n");
                    } else {
                        ms_datas_append(&s->varValues, ms_interpreter_run_code(interp, paramValue->value.param.data));
                        paramValue = paramValue->value.param.nextParam;
                        printf("NOT null.\n");
                    }
                    param = param->value.paramDef.nextParam;
                }

                // run the event
                ms_data res = ms_interpreter_run_code(interp, funcNode);

                // done
                ms_interpreter_scope_pop(interp);
                return res;
            }
        }
        s = s->parentScope;
    }

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

static void ms_interpreter_set_property(ms_interpreter *interp, const char *name, ms_data val) {

    // the interpreter context is malformed
    if (interp->context->s == NULL) {
        fprintf(stderr, "context got mangled.\n");
        exit(1);
    }

    // switch the type
    switch (interp->context->obj.type) {

        case MS_DT_COMPONENT_COLLIDER:
            if (interp->context->obj.ptr)
                ms_interpreter_component_collider_set_property(interp->context->obj.value.colliderPtr, name, val);
            else ms_interpreter_component_collider_set_property(&interp->context->obj.value.collider, name, val);
            break;
        case MS_DT_COMPONENT_VELOCITY:
            if (interp->context->obj.ptr) {
                ms_interpreter_component_velocity_set_property(interp->context->obj.value.velocityPtr, name, val);
            } else {
                ms_interpreter_component_velocity_set_property(&interp->context->obj.value.velocity, name, val);
            }
            break;
        case MS_DT_COMPONENT_MESH:
            if (interp->context->obj.ptr)
                ms_interpreter_component_mesh_set_property(interp->context->obj.value.meshPtr, name, val);
            else ms_interpreter_component_mesh_set_property(&interp->context->obj.value.mesh, name, val);
            break;
        
        case MS_DT_ENTITY:
            ms_interpreter_entity_set_property(interp->context->s, interp->context->obj.value.entity, name, val);
            break;
        
        case MS_DT_SCENE:
            ms_interpreter_scene_set_property(interp->context->obj.value.scene, name, val);
            break;
        
        case MS_DT_VEC3:
            ms_interpreter_vec3_set_property(&interp->context->obj.value.v3, name, val);
            break;

        default:
            fprintf(stderr, "obj of data type %d not implemented.\n", interp->context->obj.type);
            exit(1);
    }
}

// get a property rather than set it
static ms_data ms_interpreter_get_property(ms_interpreter *interp, const char *name) {

    // the interpreter context is malformed
    if (interp->context->s == NULL) {
        fprintf(stderr, "context got mangled.\n");
        exit(1);
    }

    // special objects
    if (interp->context->obj.type != MS_DT_NIL) {

        // switch the type
        switch (interp->context->obj.type) {
            case MS_DT_COMPONENT_COLLIDER:
                if (interp->context->obj.ptr) return ms_interpreter_component_collider_get_property(interp->context->obj.value.colliderPtr, name);
                return ms_interpreter_component_collider_get_property(&interp->context->obj.value.collider, name);
            case MS_DT_COMPONENT_VELOCITY:
                if (interp->context->obj.ptr) return ms_interpreter_component_velocity_get_property(interp->context->obj.value.velocityPtr, name);
                else return ms_interpreter_component_velocity_get_property(&interp->context->obj.value.velocity, name);
            case MS_DT_COMPONENT_MESH:
                if (interp->context->obj.ptr) return ms_interpreter_component_mesh_get_property(interp->context->obj.value.meshPtr, name);
                else return ms_interpreter_component_mesh_get_property(&interp->context->obj.value.mesh, name);

            case MS_DT_ENTITY:
                return ms_interpreter_entity_get_property(interp->context->s, interp->context->e, name);
            
            case MS_DT_SCENE:
                return ms_interpreter_scene_get_property(interp->context->obj.value.scene, name);
            
            case MS_DT_VEC3:
                return ms_interpreter_vec3_get_property(&interp->context->obj.value.v3, name);
            
            default:
                fprintf(stderr, "can't get a property on data type %d.\n", interp->context->obj.type);
                exit(1);
        }
    }

    // failure
    fprintf(stderr, "obj not implemented.\n");
    exit(1);
}

// run the set command
static ms_data ms_interpreter_run_code_cmd_set(ms_interpreter *interp, const ms_node *n) {

    // get the name
    const char *name = n->value.letCmd.name;

    // the value
    ms_data value = ms_interpreter_run_code(interp, n->value.letCmd.data);

    // set the value
    ms_interpreter_set_property(interp, name, value);

    // return the value
    return value;
}

// get a property
static ms_data ms_interpreter_run_code_cmd_get(ms_interpreter *interp, const ms_node *n) {

    // return the property
    const char *name = n->value.getCmd.name;
    return ms_interpreter_get_property(interp, name);
}

// expressions
static ms_data ms_interpreter_run_code_expression(ms_interpreter *interp, const ms_node *n) {

    // calculate the values
    ms_data left = ms_interpreter_run_code(interp, n->value.binOp.a);
    ms_data right = ms_interpreter_run_code(interp, n->value.binOp.b);

    // run the operation
    switch (n->value.binOp.operation) {
        case MS_TT_OR:
            return ms_data_or(left, right);

        case MS_TT_EQUALS:
            return ms_data_eq(left, right);
        case MS_TT_LESS_THAN:
            return ms_data_lt(left, right);
        case MS_TT_GREATER_THAN:
            return ms_data_gt(left, right);

        
        case MS_TT_PLUS:
            return ms_data_add(left, right);
        case MS_TT_MINUS:
            return ms_data_sub(left, right);
        case MS_TT_MULTIPLY:
            return ms_data_mul(left, right);
        case MS_TT_DIVIDE:
            return ms_data_div(left, right);
        
        default:
            fprintf(stderr, "can't do operation of type %d\n", n->value.binOp.operation);
            exit(1);
    }
}

// literal
static ms_data ms_interpreter_run_code_literal(ms_interpreter *interp, const ms_node *n) {
    (void)(interp);
    ms_token tok = n->value.literal;

    // create value
    switch (tok.type) {
        case MS_TT_BOOL:
            return (ms_data){ .type = MS_DT_BOOL, .ptr = FALSE, .value = (ms_data_value){ .boolean = tok.value.truthy } };
        case MS_TT_NIL:
            return (ms_data){ .type = MS_DT_NIL, .ptr = FALSE, };
        case MS_TT_NUMBER:
            return (ms_data){ .type = MS_DT_NUMBER, .ptr = FALSE, .value = (ms_data_value){ .num = tok.value.num } };
        case MS_TT_STRING:
            return (ms_data){ .type = MS_DT_STRING, .ptr = TRUE, .value = (ms_data_value){ .str = tok.value.chars } };
        case MS_TT_VEC2:
            return (ms_data){ .type = MS_DT_VEC2, .ptr = FALSE, .value = (ms_data_value){ .v2 = tok.value.v2 } };
        case MS_TT_VEC3:
            return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = tok.value.v3 } };
        case MS_TT_VEC4:
            return (ms_data){ .type = MS_DT_VEC4, .ptr = FALSE, .value = (ms_data_value){ .v4 = tok.value.v4 } };
        case MS_TT_IDENT:
            return ms_interpreter_get_variable(interp, tok.value.chars);
        default:
            fprintf(stderr, "no way to interpret literal of type %d\n", tok.type);
            exit(1);
    }
}

// run an entire "do" block
static ms_data ms_interpreter_run_code_cmd_do(ms_interpreter *interp, const ms_node *n) {

    // new context
    ms_interpreter_scope_push(interp);

    // get all of the nodes
    const ms_nodes *nodes = &n->value.doCmd.nodes;
    assert(nodes != NULL);

    // error detection
    if (nodes->length == 0) {
        ms_interpreter_scope_pop(interp);
        return ms_data_nil();
    }

    // run each node besides the last
    for (int i = 0; i < nodes->length - 1; i++) {
        ms_interpreter_run_code(interp, nodes->data[i]);
    }
    
    // return the last node
    ms_data result = ms_interpreter_run_code(interp, nodes->data[nodes->length - 1]);

    // go back a scope
    ms_interpreter_scope_pop(interp);

    // return
    return result;
}

// run an as command
static ms_data ms_interpreter_run_code_cmd_as(ms_interpreter *interp, const ms_node *n) {

    // get the new context
    ms_data newContext = ms_interpreter_run_code(interp, n->value.asCmd.who);

    // set the new context
    switch (newContext.type) {
        case MS_DT_ENTITY:
            ms_interpreter_context_push(interp, interp->context->s, newContext.value.entity, ms_data_nil());
            break;
        case MS_DT_SCENE:
            ms_interpreter_context_push(interp, newContext.value.scene, NIL_ENTITY, ms_data_nil());
            break;
        default:
            ms_interpreter_context_push(interp, interp->context->s, interp->context->e, newContext);
            break;
    }

    // run the code
    ms_interpreter_run_code(interp, n->value.asCmd.block);
    ms_data mutatedResult = interp->context->obj;

    // pop out
    ms_interpreter_context_pop(interp);

    // return the original context!
    return mutatedResult;
}

// if
static ms_data ms_interpreter_run_code_cmd_if(ms_interpreter *interp, const ms_node *n) {

    ms_data cond = ms_interpreter_run_code(interp, n->value.ifCmd.condition);

    if (cond.type != MS_DT_BOOL) {
        fprintf(stderr, "conditions must be booleans\n");
        exit(1);
    }

    if (cond.value.boolean) {
        return ms_interpreter_run_code(interp, n->value.ifCmd.block);
    } else {
        if (n->value.ifCmd.elseBlock != NULL) {
            return ms_interpreter_run_code(interp, n->value.ifCmd.elseBlock);
        }
        return ms_data_nil();
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
        case MS_NT_CMD_SET:
            return ms_interpreter_run_code_cmd_set(interp, n);
        case MS_NT_CMD_GET:
            return ms_interpreter_run_code_cmd_get(interp, n);
        case MS_NT_CMD_AS:
            return ms_interpreter_run_code_cmd_as(interp, n);
        case MS_NT_BINOP:
            return ms_interpreter_run_code_expression(interp, n);
        case MS_NT_LITERAL:
            return ms_interpreter_run_code_literal(interp, n);
        case MS_NT_CMD_ON:
            return ms_interpreter_run_code_cmd_on(interp, n);
        case MS_NT_CMD_DO:
            return ms_interpreter_run_code_cmd_do(interp, n);
        case MS_NT_CMD_IF:
            return ms_interpreter_run_code_cmd_if(interp, n);
        case MS_NT_PARAM:
            fprintf(stderr, "you cannot run a param node type: %d\n", n->value.param.data->type);
            exit(1);
        default:
            break;
    }

    // failure case
    fprintf(stderr, "no way to run type %d\n", n->type);
    exit(1);
}

// create an interpreter
ms_interpreter ms_interpreter_from(ms_ast *ast, scene *s, entity e, ms_data obj) {
    printf("interpreter started\n");

    // create the interpreter
    ms_interpreter interp = (ms_interpreter){
        .scope = ms_interpreter_scope_new(NULL),
        .context = ms_interpreter_context_new(s, e, obj, NULL),
        .ast = ast
    };

    // load the functions
    for (int i = 0; i < ast->nodes.length; i++) {
        const ms_node *n = ast->nodes.data[i];

        // run the code!
        ms_interpreter_run_code(&interp, n);
    }

    // return the interpreter
    return interp;
}

// run all the update functions
static void ms_interpreter_call_all_frame_fns_for_scope(ms_interpreter *interp, ms_interpreter_scope *s) {
    if (s == NULL) return;
    for (int i = 0; i < s->funcNames.length; i++) {
        if (strcmp(s->funcNames.data[i], "frame") == 0) {
            ms_interpreter_run_code_cmd_do(interp, s->funcNodes.data[i]);
        }
    }
    ms_interpreter_call_all_frame_fns_for_scope(interp, s->parentScope);
}
void ms_interpreter_call_all_frame_fns(ms_interpreter *interp) {
    ms_interpreter_call_all_frame_fns_for_scope(interp, interp->scope);
}