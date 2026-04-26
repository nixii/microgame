
#ifndef MICROENGINE_CAMERA_H
#define MICROENGINE_CAMERA_H

#include "microengine/component/transform.h"
#include "microengine/math.h"

typedef struct me_camera {
    me_transform transform;
    float fov;
} me_camera;

// make a new camera
me_camera me_camera_new(float fov, me_transform tf);

#endif // MICROENGINE_CAMERA_H