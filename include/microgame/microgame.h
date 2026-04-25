
#ifndef MICROGAME_H
#define MICROGAME_H

#include <stdlib.h>
#include <microengine/renderer.h>

/*
 * store all the information important to the game
 * only one window allowed.
*/
typedef struct microgame microgame;

/*
 * create a new game
 * this will create a renderer too
*/
microgame *microgame_new(int width, int height, char *title, int framerate);

/*
 * is the game still running
 * this is a slight expansion over raylib's WindowShouldClose() method
*/
int microgame_running(microgame *game);

/*
 * actually render!
*/
void microgame_render(microgame *game);

/*
 * fully destroy the game instance
 * you really should run thissss
*/
void microgame_destroy(microgame *game);

#endif // MICROGAME_H