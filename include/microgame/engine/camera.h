
#ifndef MG_CAMERA_H
#define MG_CAMERA_H

#include "microgame/util/transform.h"
#include "microgame/util/math.h"
#include "microgame/util/triangle.h"

#define NEAR_CLIP 0.1
#define NEAR_CLIP_EPSILON 1e-4f

typedef struct camera {
    transform transform;
    float fov;
} camera;

typedef struct projection_result {
    int failure;
    vec3 vec;
} projection_result;

typedef struct camera_translation_result {
    triangle triangles[2];
    int numTriangles;
} camera_translation_result;

camera camera_new();
vec3 camera_transform(camera *c, vec3 p);
projection_result camera_project_point(camera *c, vec3 p, int width, int height);
camera_translation_result camera_translate_triangle(camera *c, vec3 v1, vec3 v2, vec3 v3);

#endif // MG_CAMERA_H