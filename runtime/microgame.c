
#include <raylib.h>
#include "microgame/microgame.h"

struct microgame {

    // metadata
    int width;
    int height;
    int framerate;
    char *title;

    // engine objects
    me_renderer _renderer;

    // raylib objects
    Image _bufferImage;
    Texture2D _bufferTexture;
};

/*
 * create a new game
 * this will create a renderer too
*/
microgame *microgame_new(int width, int height, char *title, int framerate)
{

    // create the pointer
    microgame *game = malloc(sizeof(microgame));

    // set metadata
    game->width = width;
    game->height = height;
    game->framerate = framerate;
    game->title = title;

    // initialize the window and stuff
    InitWindow(game->width, game->height, game->title);
    SetTargetFPS(game->framerate);

    // create the renderer
    game->_renderer = me_renderer_new(width, height);

    // create the image the pixels render to
    game->_bufferImage = (Image){
        .data = game->_renderer.pixels,
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };

    // create the texture they render to
    game->_bufferTexture = LoadTextureFromImage(game->_bufferImage);

    // return the game
    return game;
}

/*
 * is the game still running
 * this is a slight expansion over raylib's WindowShouldClose() method
*/
int microgame_running(microgame *game)
{
    return !WindowShouldClose();
}

/*
 * update the texture and draw it to the screen
*/
void microgame_render(microgame *game)
{

    // render
    me_renderer_clear(&game->_renderer, 0x00000000);

    // TODO: remove this
    me_renderer_render_square(&game->_renderer, 10, 10, 150, 150, 0xFFFF00FF);

    // update the texture
    UpdateTexture(game->_bufferTexture, game->_renderer.pixels);

    // start draw calls
    BeginDrawing();

    {
        DrawTexture(game->_bufferTexture, 0, 0, WHITE);
    }

    // end draw calls
    EndDrawing();

}

/*
 * fully destroy the game instance
 * you really should run thissss
*/
void microgame_destroy(microgame *game)
{

    // destroy the renderer
    me_renderer_destroy(&game->_renderer);

    // free the window
    CloseWindow();
}