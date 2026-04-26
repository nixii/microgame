
#ifndef MG_CAMERA_H
#define MG_CAMERA_H

#include "microgame/util/transform.h"

typedef struct camera {
    transform transform;
    float fov;
} camera;

camera camera_new();

#endif // MG_CAMERA_H