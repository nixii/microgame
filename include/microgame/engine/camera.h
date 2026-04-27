
#ifndef MG_CAMERA_H
#define MG_CAMERA_H

#include "microgame/util/transform.h"
#include "microgame/util/math.h"

typedef struct camera {
    transform transform;
    float fov;
} camera;

camera camera_new();
vec3 camera_transform(camera *c, vec3 p);
vec2 camera_project_point(camera *c, vec3 p, int width, int height);

#endif // MG_CAMERA_H