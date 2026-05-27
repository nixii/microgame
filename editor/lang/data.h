
#ifndef MS_DATA_H
#define MS_DATA_H

#include "../da.h"

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
    
    scene *scene;
    entity entity;
} ms_data_value;

// a data point
typedef struct {
    ms_data_type type;
    ms_data_value value;
} ms_data;

// declare the data dyn arr
DA_DECLARE(ms_datas, ms_data);

#endif // MS_DATA_H