
#include "util.h"

static mesh_resource topGroundMeshResource;
static mesh_resource bottomGroundMeshResource;
static mesh_resource npcMeshResource;
static font_resource mainFont;

static void load_meshes() {
    topGroundMeshResource = mesh_resource_from_obj("testgame1/assets/ground1_top.obj");
    bottomGroundMeshResource = mesh_resource_from_obj("testgame1/assets/ground1_bottom.obj");
    npcMeshResource = mesh_resource_from_obj("testgame1/assets/npc.obj");
    mainFont = font_resource_from("assets/simplefont.png", 16, 24, -1, 3);
}

static int loadedSceneUtil = 0;
void load_scene_util() {
    if (!loadedSceneUtil) {
        load_meshes();
    }
}

void spawn_platform(scene *s, vec3 pos, vec3 scale) {
    if (!loadedSceneUtil) load_scene_util();
    entity e = scene_spawn(s);
    entity e2 = scene_spawn(s);
    
    set_parent(s, e2, e);

    attach_mesh(s, e, mesh_from_resource(rgb(147, 241, 147), topGroundMeshResource));
    attach_mesh(s, e2, mesh_from_resource(rgb(91, 38, 77), bottomGroundMeshResource));
    attach_collider(s, e, collider_new(vec3_new(4, 2, 4)));

    get_transform(s, e)->pos = pos;
    get_transform(s, e)->scale = scale;
}

entity spawn_player(scene *s, vec3 pos) {
    if (!loadedSceneUtil) load_scene_util();
    entity p = scene_spawn(s);
    
    attach_collider(s, p, collider_new(vec3_new(0.5, 1, 0.5)));
    attach_velocity(s, p, velocity_new(vec3_zero()));

    get_transform(s, p)->pos = pos;

    return p;
}

void create_ui(scene *s) {

    ui_container *root = ui_container_empty();
    root->size = ui_vec_new(1, 0, 1, 0);

    ui_container *dialogueCover = ui_container_empty();
    dialogueCover->size = ui_vec_new(0.5, 0, 0.3, 0);
    dialogueCover->anchor = vec2_new(0.5, 1);
    dialogueCover->pos = ui_vec_new(0.5, 0, 1, 0);
    ui_container_bind_type(dialogueCover, UI_TYPE_RECT, ui_rect_new(rgb(65, 31, 65)));
    ui_container_set_parent(dialogueCover, root);

    ui_container *dialogue = ui_container_empty();
    dialogue->size = ui_vec_new(1, -8, 1, -8);
    dialogue->anchor = vec2_new(0.5, 0.5);
    dialogue->pos = ui_vec_new(0.5, 0, 0.5, 0);
    ui_text *txt = ui_text_new(&mainFont, "Basic dialogue box here, it will have use and be able to turn off eventually.");
    txt->autoWrap = 1;
    ui_container_bind_type(dialogue, UI_TYPE_TEXT, txt);
    ui_container_set_parent(dialogue, dialogueCover);

    scene_set_ui_root(s, root);
}