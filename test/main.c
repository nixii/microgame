
#include <microgame/microgame.h>

int main(void)
{

    microgame *game = microgame_new(600, 400, "hello world", 60);

    while (microgame_running(game))
    {
        microgame_render(game);
    }

    microgame_destroy(game);
    return 0;
}