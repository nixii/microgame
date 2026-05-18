
#include "microgame/engine/resource/sound.h"

sound sound_from(const char *path) {
    return (sound) {
        ._rlSound = LoadSound(path)
    };
}

void sound_play(sound *s) {
    PlaySound(s->_rlSound);
}

void sound_destroy(sound *s) {
    UnloadSound(s->_rlSound);
}