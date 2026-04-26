
#include "microgame/util/transform.h"

/************************
 * transform functions
*/

// create a new transform
transform transform_new(vec3 pos, vec3 rot, vec3 scale) {
    return (transform){ .pos = pos, .rot = rot, .scale = scale };
}

// create a new empty transform (pos/rot 0, scale 1)
transform transform_blank() {
    return transform_new(vec3_zero(), vec3_zero(), vec3_new(1, 1, 1));
}