
#include "microgame/renderer/renderer.h"
#include "microgame/engine/systems.h"
#include "microgame/engine/scene.h"
#include "microgame/util/color.h"
#include <stdlib.h>
#include <stdio.h>

// create a new scene
scene *scene_new() {

    // allocate the entire scene to 0
    scene *s = calloc(1, sizeof(scene));

    // create a new camera
    s->camera = camera_new();

    // clear the root
    s->uiRoot = NULL;

    // vertices ready to render
    s->triangleBufferSize = INIT_TRIANGLE_COUNT;
    s->triangleBuffer = (triangle *)malloc(sizeof(triangle) * s->triangleBufferSize);

    // load the free IDs
    for (s->lastFree = 0; s->lastFree < MAX_ENTITIES; s->lastFree++) {
        s->freeIds[s->lastFree] = s->lastFree;
        s->parents[s->lastFree] = NIL_ENTITY;
    }

    // correct index for the last free ID
    s->lastFree = MAX_ENTITIES - 1;

    // return the heap-allocated scene
    return s;
}

// spawn an entity
entity scene_spawn(scene *s) {

    // if there is no room then don't spawn a new entity
    if (s->lastFree < 0)
        return NIL_ENTITY;

    // get the next id and decrement the amount free
    entity id = s->freeIds[s->lastFree--];

    // alive is true
    s->alive[id] = 1;

    // create the base objects
    s->transforms[id] = transform_blank();
    s->parents[id] = NIL_ENTITY;

    // return the entity's ID
    return id;
}

// despawn an entity
void scene_despawn(scene *s, entity e) {
    if (s->alive[e] != 1)
        return;
    s->alive[e] = 0;
    s->freeIds[++s->lastFree] = e;
    s->parents[s->lastFree] = NIL_ENTITY;

    // destroy any components
#define X(name, type)\
    detach_##name(s, e);
    X_COMPONENTS
#undef X
}

// destroy a whole scene
void scene_destroy(scene *s) {

    // destroy components
    for (entity e = 0; e < MAX_ENTITIES; e++) {

        // if the entity is not alive then ignore it
        if (!s->alive[e]) continue;

        // destroy each component (X-macro)
    #define X(name, type)\
        detach_##name(s, e);
        X_COMPONENTS
    #undef X
    }

    // destroy the scene
    free(s->triangleBuffer);
    free(s);
}

// get a transform
transform *get_transform(scene *s, entity e) {
    if (!s->alive[e]) return NULL;
    return &s->transforms[e];
}

// component functions
#define X(name, type) \
    int has_##name(scene *s, entity e) { \
        return s->has_##name[e]; \
    } \
    type *get_##name(scene *s, entity e) { \
        if (!has_##name(s, e)) return NULL; \
        return &s->name##_components[e]; \
    } \
    type *attach_##name(scene *s, entity e, type c) { \
        s->has_##name[e] = 1; \
        s->name##_components[e] = c; \
        return &s->name##_components[e]; \
    } \
    void detach_##name(scene *s, entity e) { \
        s->has_##name[e] = 0; \
        name##_destroy(&s->name##_components[e]); \
    }
X_COMPONENTS
#undef X

// get the world transform of an entity
static mat4 get_world_transform_mat4(scene *s, entity e) {

    // if there is a global matrix then return it
    if (s->hasGlobalMat[e])
        return s->globalMats[e];

    // get the transform
    transform childTransform = s->transforms[e];

    // get the model matrix for the child
    mat4 childMatrix = mat4_model(childTransform.pos, childTransform.rot, childTransform.scale);

    // no parent = no confusion
    if (get_parent(s, e) == NIL_ENTITY) {
        return childMatrix;
    }

    // parent model matrix
    entity parent = get_parent(s, e);

    // parent transform
    mat4 parentMatrix = get_world_transform_mat4(s, parent);

    // set the global matrix and perform the operation
    s->hasGlobalMat[e] = 1;
    s->globalMats[e] = mat4_mul(parentMatrix, childMatrix);

    // return the matrix
    return s->globalMats[e];
}   

// get world transform
transform get_global_transform(scene *s, entity e) {
    mat4 m = get_world_transform_mat4(s, e);

    vec3 pos = vec3_new(
        m.m[0][3],
        m.m[1][3],
        m.m[2][3]
    );

    vec3 scale = vec3_new(
        vec3_len(vec3_new(m.m[0][0], m.m[1][0], m.m[2][0])),
        vec3_len(vec3_new(m.m[0][1], m.m[1][1], m.m[2][1])),
        vec3_len(vec3_new(m.m[0][2], m.m[1][2], m.m[2][2]))
    );

    // TODO: detect rotation
    return transform_new(
        pos,
        vec3_zero(),
        scale
    );
}

// render a scene!
void scene_render(
    scene *s, 
    renderer *r, 
    vec2 mousePos, 
    int mouseLeftDown, 
    float dt)
{

    // load global transform matrices
    for (entity e = 0; e < MAX_ENTITIES; e++) {
        if (!s->alive[e]) continue;
        get_world_transform_mat4(s, e);
    }

    // update the systems
    collision_system_update(s, dt);

    // update ui first
    if (s->uiRoot)
        ui_container_update(s->uiRoot, mousePos, mouseLeftDown, 0, 0, r->width, r->height);

    // list of all triangles
    int numTris = 0;

    // for each entity
    for (entity e = 0; e < MAX_ENTITIES; e++) {
        if (!s->alive[e]) continue;

        // load its transform
        mat4 tm = get_world_transform_mat4(s, e);

        // get the mesh
        mesh *m = get_mesh(s, e);

        // for each triangle
        if (m != NULL) {
            for (int i = 0; i < m->numIndices; i += 3) {

                // transform the vertex positions
                mat4 m1 = mat4_mul(tm, mat4_translation(m->vertices[m->indices[i]]));
                mat4 m2 = mat4_mul(tm, mat4_translation(m->vertices[m->indices[i + 1]]));
                mat4 m3 = mat4_mul(tm, mat4_translation(m->vertices[m->indices[i + 2]]));

                // global locs
                vec3 v1 = vec3_new(m1.m[0][3], m1.m[1][3], m1.m[2][3]);
                vec3 v2 = vec3_new(m2.m[0][3], m2.m[1][3], m2.m[2][3]);
                vec3 v3 = vec3_new(m3.m[0][3], m3.m[1][3], m3.m[2][3]);
                
                // translate and clip the triangles
                camera_translation_result tris = camera_translate_triangle(&s->camera, v1, v2, v3);
                if (tris.numTriangles == 0) continue;

                // color the triangles
                for (int i = 0; i < tris.numTriangles; i++)
                    tris.triangles[i].color = m->color;

                // expand if the amount of triangles to add is too many
                if (numTris + tris.numTriangles >= s->triangleBufferSize) {
                    s->triangleBufferSize *= 2;
                    s->triangleBuffer = realloc(s->triangleBuffer, sizeof(triangle) * s->triangleBufferSize);
                }

                // add it in
                for (int i = 0; i < tris.numTriangles; i++)
                    s->triangleBuffer[numTris++] = tris.triangles[i];
            }
        }
    }
    
    // draw each triangle!
    for (int i = 0; i < numTris; i++) {

        // get the triangle
        triangle t = s->triangleBuffer[i];

        // project the points to 2d (but keep the z)
        vec3 v1 = camera_project_point(&s->camera, t.a, r->width, r->height);
        vec3 v2 = camera_project_point(&s->camera, t.b, r->width, r->height);
        vec3 v3 = camera_project_point(&s->camera, t.c, r->width, r->height);

        // skip behind the camera
        if ((v1.x == v1.y && v1.x == -1) ||
            (v2.x == v2.y && v2.x == -1) ||
            (v3.x == v3.y && v3.x == -1))
            continue;

        // render the triangle
        float lighting = (vec3_dot(t.normal, vec3_new(0, 1, 0)) + 1) / 5.0;
        renderer_render_triangle(r, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, rgb_mix(t.color, rgb(lighting * 256, lighting * 256, lighting * 256)));
    }

    // render the ui
    if (s->uiRoot != NULL)
        ui_container_render(s->uiRoot, r, 0, 0, r->width, r->height);

    // reset the global matrix per-frame cache
    for (entity i = 0; i < MAX_ENTITIES; i++) {
        s->hasGlobalMat[i] = 0;
    }
}   

// get the parent of an entity
entity get_parent(scene *s, entity child) {
    return s->parents[child];
}

// set the parent of an entity
void set_parent(scene *s, entity child, entity parent) {
    s->parents[child] = parent;
}

// remove the parent of an entity
void remove_parent(scene *s, entity child) {
    set_parent(s, child, NIL_ENTITY);
}

// set the ui root of the scene
void scene_set_ui_root(scene *s, ui_container *c) {
    
    // if you are setting the root to null, remove the UI
    if (c == NULL) {
        s->uiRoot = NULL;
        return;
    }

    // set the UI root
    s->uiRoot = c;
}

// get the ui root, maybe NULL
ui_container *scene_get_ui_root(scene *s) {

    // simply return the root!
    return s->uiRoot;
}