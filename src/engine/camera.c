
#include "microgame/engine/camera.h"

camera camera_new() {
    return (camera) {
        .fov = 3.14 / 2,
        .transform = transform_blank()
    };
}