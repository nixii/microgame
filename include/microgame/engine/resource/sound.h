
#ifndef MG_SOUND_RESOURCE_H
#define MG_SOUND_RESOURCE_H

#include <raylib.h>

typedef struct sound {
    Sound _rlSound;
} sound;

sound sound_from(const char *path);
void sound_play(sound *s);
void sound_destroy(sound *s);

#endif // MG_SOUND_RESOURCE_H