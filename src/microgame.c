
#include <raylib.h>
#include <stdlib.h>
#include "microgame/microgame.h"
#include "microgame/renderer/renderer.h"
#include "microgame/engine/scene.h"
#include <stdio.h>

// SPECIFY THE TYPE
struct microgame {
    int width;
    int height;
    int fps;
    char *title;

    // render things
    renderer renderer;

    // store the main scene
    scene *mainScene;

    // raylib objects
    Image _bufImg;
    Texture2D _bufTex;
};

// create a new game instance
microgame *game_new(int width, int height, int fps, char *title) {

    // create the base object
    microgame *g = malloc(sizeof(microgame));
    g->width = width;
    g->height = height;
    g->fps = fps;
    g->title = title;
    g->mainScene = NULL;

    // create the renderer
    g->renderer = renderer_new(width, height);

    // init the window
    InitWindow(width, height, title);
    SetTargetFPS(fps);

    // create the base image
    g->_bufImg = (Image){
        .data = g->renderer.pixels,
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };

    // create the texture
    g->_bufTex = LoadTextureFromImage(g->_bufImg);

    // all good!
    return g;
}

// set the scene
void game_set_scene(microgame *g, scene *s) {
    g->mainScene = s;
}

// while running
int game_still_running(microgame *g) {
    return (g != NULL && !WindowShouldClose());
}

// update the game
void game_update(microgame *g) {

    // clear the screen
    renderer_clear(&g->renderer);

    // render the scene
    if (g->mainScene) {
        scene_render(g->mainScene, &g->renderer);
    }

    // update the texture
    UpdateTexture(g->_bufTex, g->renderer.pixels);
    BeginDrawing();
    DrawTexture(g->_bufTex, 0, 0, WHITE);
    DrawText(TextFormat("%d FPS", GetFPS()), 10, 10, 30, WHITE); // TODO: remove FPS counter
    EndDrawing();
}

// destroy the game
void game_destroy(microgame *g) {
    CloseWindow();
    free(g);
}

// get the deltatime (for lag help!)
float get_dt() {
    return GetFrameTime();
}

// key presses
int key_down(key k) {
    return IsKeyDown(key_to_rl(k));
}
int key_just_down(key k) {
    return IsKeyPressed(key_to_rl(k));
}
int key_just_up(key k) {
    return IsKeyReleased(key_to_rl(k));
}