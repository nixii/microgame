
#include <microgame/microgame.h>
#include "scenes/l1.h"

// window settings
#define WIDTH 1200
#define HEIGHT 800
#define FPS 60
#define TITLE "Platformer!"

// main loop!
int main(void) {

    // create the game
    microgame *game = game_new(WIDTH, HEIGHT, FPS, TITLE);

    // load the scenes
    prepare_scene_1();

    // set the main scene
    game_set_scene(game, get_scene_1());

    // while the game is running
    while (game_still_running(game)) {
        game_update(game);
    }

    // destroy the game
    game_destroy(game);
}