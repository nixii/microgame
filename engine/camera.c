
#include "microengine/camera.h"

me_camera me_camera_new(float fov, me_transform tf)
{
    return (me_camera){
        .fov = fov,
        .transform = tf
    };
}