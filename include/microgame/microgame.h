
#ifndef MICROGAME_H
#define MICROGAME_H

#include "microgame/engine/scene.h"
#include "microgame/input.h"
#define MAX_INPUTS 8

// Don't give more info on the type
typedef struct microgame microgame;

// Functions
microgame *game_new(int width, int height, int fps, char *title);
void game_set_scene(microgame *g, scene *s);
int game_still_running(microgame *g);
void game_update(microgame *g);
void game_destroy(microgame *g);
int key_down(key k);
int key_just_down(key k);
int key_just_up(key k);
float get_dt();
int get_fps();

#endif // MICROGAME_H