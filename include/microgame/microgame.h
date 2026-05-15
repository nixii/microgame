
#ifndef MICROGAME_H
#define MICROGAME_H

#include "microgame/engine/scene.h"
#include "microgame/input.h"
#define MAX_INPUTS 8
#define PI 3.1415926535

// Don't give more info on the type
typedef struct microgame microgame;

// Functions
microgame *game_new(int width, int height, int fps, char *title);
void game_set_scene(microgame *g, scene *s);
int game_still_running(microgame *g);
void game_update(microgame *g);
void game_destroy(microgame *g);
float get_dt();
int get_fps();

// input functions
int key_down(key k);
int key_just_down(key k);
int key_just_up(key k);
int mouse_down(int mouse);
int mouse_just_down(int mouse);
int mouse_just_up(int mouse);

void disable_mouse();
void enable_mouse();
vec2 get_mouse_delta();

#endif // MICROGAME_H