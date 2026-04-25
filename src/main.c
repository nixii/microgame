
#include "render.h"
#include "color.h"
#include <raylib.h>

int main(void)
{

    // create the window
    InitWindow(600, 400, "test window");
    SetTargetFPS(30);

    // create the renderer
    renderer r = renderer_new(600, 400);

    // create the image for the renderer
    Image img = {
        .data = r.pixels,
        .width = r.width,
        .height = r.height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };

    // load the image to a texture
    Texture2D tex = LoadTextureFromImage(img);

    // build the triangle to render
    int triangle[] = {
        10, 10,
        300, 10,
        10, 399
    };

    // while the game is running
    while (!WindowShouldClose())
    {

        // update the renderer
        renderer_clear(&r);
        renderer_render_triangle(&r, triangle, rgb(99, 207, 180));
        triangle[4] += 1;

        // update the texture
        UpdateTexture(tex, r.pixels);

        // draw!
        BeginDrawing();

            // blit the renderer
            DrawTexture(tex, 0, 0, WHITE);

        EndDrawing();
    }

    // cleanup
    CloseWindow();

    // all good!
    return 0;
}