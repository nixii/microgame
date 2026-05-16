
#include <microgame/microgame.h>

void load_scene_util();

void spawn_platform(scene *s, vec3 pos, vec3 scale);
entity spawn_player(scene *s, vec3 pos);
entity spawn_npc(scene *s, vec3 pos, float rot);

void create_ui(scene *s);

ui_container *get_dialogue_container();
ui_text *get_dialogue();
void set_dialogue_container_visibility(scene *s, int visible);