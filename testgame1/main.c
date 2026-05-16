
#include <microgame/microgame.h>
#include "movement/movement.h"
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
    disable_mouse();

    // load the scenes
    entity player = prepare_scene_1();

    // set the main scene
    scene *sc = get_scene_1();
    game_set_scene(game, sc);

    // while the game is running
    while (game_still_running(game)) {
        update_movement(sc, player);
        game_update(game);
    }

    // destroy the game
    game_destroy(game);
    destroy_scene_1();
}