
#ifndef MICROGAME_H
#define MICROGAME_H

#include <microengine/scene.h>

typedef struct game game;

game *microgame_init(int width, int height, char *title, int fps);
void microgame_set_scene(game *g, me_scene *s);
int microgame_running(game *g);
void microgame_render(game *g);
void microgame_end(game *g);

#endif // MICROGAME_H