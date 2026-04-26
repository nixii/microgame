
#ifndef MICROENGINE_H
#define MICROENGINE_H

#include <microrender/renderer.h>
#include "microengine/scene.h"

void microengine_render(me_scene *s, mr_renderer *r);

#endif // MICROENGINE_H