
#include <stdio.h>
#include <microgame/microgame.h>
#include "lang/lexer.h"
#include "lang/parser.h"
#include "lang/interpret.h"

#define WIDTH 1080
#define HEIGHT 720
#define FPS 60
#define TITLE "Microgame!"

int main(void) {

    // create the game
    microgame *game = game_new(WIDTH, HEIGHT, FPS, TITLE);

    // the main scene
    scene *s = scene_new();

    // load the script
    ms_tokens tokens = tokenize("designing_the_engine/test.microscript");
    ms_ast ast = parse(&tokens);

    // interpret!
    ms_interpreter interp = ms_interpreter_from(&ast, s, -1);

    // run
    while (game_still_running(game)) {
        game_update(game);
    }

    // end
    scene_destroy(s);
    game_destroy(game);
    return 0;
}