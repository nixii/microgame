
#include <microgame/microgame.h>
#include "l1.h"

// the scene thing
static scene *_s1;

// prepare the first scene
void prepare_scene_1() {

    // create the scene
    _s1 = scene_new();
}

// get the scene
scene *get_scene_1() {

    // return the scene
    return _s1;
}