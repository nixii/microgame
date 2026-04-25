
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "microgame/microgame.h"
#include "microrender/renderer.h"
#include "microengine/scene.h"
#include "microengine/math.h"

typedef struct game {
    int width;
    int height;
    char *title;
    int fps;

// the renderer
    mr_renderer renderer;

// engine objects
    me_scene *mainScene;

// raylib objects
    Image bufImg;
    Texture2D bufTex;
} game;

game *microgame_init(int width, int height, char *title, int fps)
{

    // allocate the game
    game *g = malloc(sizeof(game));
    g->mainScene = NULL;

    // store the data
    g->width = width;
    g->height = height;
    g->title = title;
    g->fps = fps;

    // initialize the window
    InitWindow(g->width, g->height, g->title);
    SetTargetFPS(g->fps);

    // create the renderer
    g->renderer = mr_renderer_new(g->width, g->height);

    // create the raylib objects
    g->bufImg = (Image){
        .data = g->renderer.pixels,
        .width = g->width,
        .height = g->height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };
    g->bufTex = LoadTextureFromImage(g->bufImg);

    return g;
}

void microgame_set_scene(game *g, me_scene *s)
{
    g->mainScene = s;
}

int microgame_running(game *g)
{
    return !WindowShouldClose();
}

void microgame_render(game *g)
{

    // Clear the screen
    mr_renderer_clear(&g->renderer, mr_rgb(0, 0, 0));

    // Draw each entity as a square
    for (int i = 0; i < ME_MAX_ENTITIES; i++)
    {
        if (!g->mainScene->alive[i]) continue;
        if (!g->mainScene->has_transform[i]) continue;
        if (!g->mainScene->has_mesh[i]) continue;

        me_vec3 t = g->mainScene->transform_components[i].pos;
        me_mesh m = g->mainScene->mesh_components[i];

        for (int i = 0; i < m.triCount; i++)
        {
            mr_renderer_render_triangle(&g->renderer, 
                m.points[i * 3].x, m.points[i * 3].y,
                m.points[i * 3 + 1].x, m.points[i * 3 + 1].y,
                m.points[i * 3 + 2].x, m.points[i * 3 + 2].y,
                mr_rgb(255, 0, 255)
            );
        }
    }
    
    // Draw the texture
    UpdateTexture(g->bufTex, g->renderer.pixels);
    BeginDrawing();
    DrawTexture(g->bufTex, 0, 0, WHITE);
    EndDrawing();
}

void microgame_end(game *g)
{
    mr_renderer_destroy(&g->renderer);
    CloseWindow();
}