
#include "microgame/engine/components/mesh.h"
#include "microgame/engine/resource/image.h"
#include "microgame/engine/resource/mesh.h"
#include "microgame/engine/ui/container.h"
#include "microgame/engine/scene.h"
#include "microgame/util/color.h"
#include "microgame/util/math.h"
#include "microgame/microgame.h"
#include <stdio.h>
#include <math.h>

void warning(entity e, entity hitBy) {
    printf("Oh no %d hit me!\n", hitBy);
}

int main(void)
{

    // create the game
    microgame *g = game_new(1600, 1000, 60, "hi world");
    scene *s = scene_new();

    // load the mesh
    mesh_resource mr = mesh_resource_from_obj("assets/test.obj");
    mesh m = mesh_from_resource(rgb(255, 255, 255), mr);

    // load the text
    font_resource fr = font_resource_from("assets/simplefont.png", 16, 24, -1, 3);

    // get the transform
    camera *c = &s->camera;
    c->transform.pos = vec3_new(0, 3, -10);

    // make UI
    ui_container *root = ui_container_empty();
    root->pos = ui_vec_new(0, 10, 0, 10);

    // bind text to the root
    ui_text *t = ui_text_new(&fr, "000 FPS");
    ui_container_bind_type(root, UI_TYPE_TEXT, t);

    // add colliders
    entity e1 = scene_spawn(s);
    entity e2 = scene_spawn(s);
    collider *a = scene_attach_collider(s, e1, collider_new(vec3_new(1, 1, 1)));
    scene_attach_collider(s, e2, collider_new(vec3_new(1, 1, 1)));
    scene_attach_mesh(s, e1, m);
    scene_attach_mesh(s, e2, m);
    transform *fT = get_transform(s, e1);
    fT->pos.x = 1;
    get_transform(s, e2)->pos.x = -1;
    get_transform(s, e1)->scale = vec3_new(0.8, 0.8, 0.8);
    get_transform(s, e2)->scale = vec3_new(0.8, 0.8, 0.8);

    // set functions
    a->onCollision = warning;

    // set the UI
    scene_set_ui_root(s, root);

    // set the scene
    game_set_scene(g, s);
    
    // speed
    float camSpeed = 1.7; // 1.7 rad a second
    float moveSpeed = 5.0; // 5 meters a second
    int frameCount = 0;
    
    // update
    while (game_still_running(g)) {

        // dt
        float dt = get_dt();

        // update fps
        char buf[16];
        snprintf(buf, sizeof(buf), "%d FPS", get_fps());
        ui_text_set_text(t, buf);

        // rotate the camera
        float rotYaw = (float)(key_down(M_KEY_RIGHT) - key_down(M_KEY_LEFT)) * camSpeed * dt;
        c->transform.rot.y += rotYaw;
        float rotPitch = (float)(key_down(M_KEY_DOWN) - key_down(M_KEY_UP)) * camSpeed * dt;
        c->transform.rot.x += rotPitch;

        // get the cam movement
        vec3 movement = vec3_zero();
        if (key_down(M_KEY_A))
            movement.x -= moveSpeed * dt;
        if (key_down(M_KEY_D))
            movement.x += moveSpeed * dt;
        if (key_down(M_KEY_W))
            movement.z += moveSpeed * dt;
        if (key_down(M_KEY_S))
            movement.z -= moveSpeed * dt;
        if (key_down(M_KEY_SPACE))
            movement.y += moveSpeed * dt;
        if (key_down(M_KEY_LCTRL))
            movement.y -= moveSpeed * dt;
        
        // rotate the vector
        movement = vec3_rotY(movement, c->transform.rot.y);
        c->transform.pos = vec3_add(c->transform.pos, movement);
        
        // frame count!
        fT->pos.x = sinf((float)frameCount / 60);
        frameCount++;
        
        // update the game
        game_update(g);
    }

    // end
    game_destroy(g);
}