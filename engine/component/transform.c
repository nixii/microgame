
#include "microengine/component/transform.h"

me_transform me_transform_new(me_vec3 pos, me_vec3 rot, me_vec3 scale)
{
    return (transform){
        .pos = pos,
        .rot = rot,
        .scale = scale
    };
}

me_transform me_transform_blank()
{
    return me_transform_new(
        me_vec3_new(0, 0, 0),
        me_vec3_new(0, 0, 0),
        me_vec3_new(1, 1, 1)
    );
}