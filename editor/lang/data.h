
#ifndef MS_DATA_H
#define MS_DATA_H

#include <microgame/microgame.h>
#include "../da.h"

#ifndef FALSE
#   define FALSE 0
#endif
#ifndef TRUE
#   define TRUE 1
#endif

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
    MS_DT_ENTITY,
    MS_DT_COMPONENT_COLLIDER,
    MS_DT_COMPONENT_MESH,
    MS_DT_COMPONENT_VELOCITY,

    // resources
    MS_DT_RESOURCE_MESH,
} ms_data_type;

// a ms value
typedef union {
    const char *str;
    float num;
    int boolean;

    vec2 v2;
    vec2 *v2Ptr;
    vec3 v3;
    vec3 *v3Ptr;
    ui_vec v4;
    ui_vec *v4Ptr;
    
    scene *scene;
    entity entity;

    collider collider;
    collider *colliderPtr;
    velocity velocity;
    velocity *velocityPtr;
    mesh mesh;
    mesh *meshPtr;

    mesh_resource meshResource;
    mesh_resource *meshResourcePtr;
} ms_data_value;

// a data point
typedef struct {
    ms_data_type type;
    ms_data_value value;
    uint8_t ptr;
} ms_data;

// declare the data dyn arr
DA_DECLARE(ms_datas, ms_data);

// perform operations
ms_data ms_data_mul(ms_data a, ms_data b);
ms_data ms_data_div(ms_data a, ms_data b);
ms_data ms_data_add(ms_data a, ms_data b);
ms_data ms_data_sub(ms_data a, ms_data b);
ms_data ms_data_eq(ms_data a, ms_data b);
ms_data ms_data_lt(ms_data a, ms_data b);
ms_data ms_data_gt(ms_data a, ms_data b);
ms_data ms_data_or(ms_data a, ms_data b);

ms_data ms_data_bool(int truthy);
ms_data ms_data_number(float num);
ms_data ms_data_vec2(vec2 v2);
ms_data ms_data_vec3(vec3 v3);
ms_data ms_data_vec4(ui_vec v4);

#define MS_DATA(a) _Generic((a), \
    float: ms_data_number, \
    int: ms_data_bool, \
    vec2: ms_data_vec2, \
    vec3: ms_data_vec3, \
    ui_vec: ms_data_vec4)(a)

#endif // MS_DATA_H