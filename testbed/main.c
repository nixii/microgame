
#include "microgame/engine/components/mesh.h"
#include "microgame/engine/resource/image.h"
#include "microgame/engine/resource/mesh.h"
#include "microgame/engine/ui/container.h"
#include "microgame/util/color.h"
#include "microgame/util/math.h"
#include "microgame/microgame.h"
#include <stdio.h>
#include <math.h>

int main(void)
{

    // create the game
    microgame *g = game_new(1600, 1000, 60, "hi world");
    scene *s = scene_new();

    // load the mesh
    mesh_resource mr = mesh_resource_from_obj("assets/test.obj");
    mesh m = mesh_from_resource(rgb(255, 255, 255), mr);

    // load the text
    font_resource fr = font_resource_from("assets/simplefont.png", 16, 24, 1, 3);

    // make one stationary test entity
    entity stationary = scene_spawn(s);
    scene_attach_mesh(s, stationary, m);

    // get the transform
    camera *c = &s->camera;
    c->transform.pos = vec3_new(0, 3, -10);

    // make UI
    ui_container *root = ui_container_empty();
    root->size = ui_vec_new(1, 0, 1, 0);
    root->pos = ui_vec_new(0.5, 0, 0.5, 0);
    
    // add text
    ui_text *ut = ui_text_new(&fr, "150%");
    ui_container_bind_type(root, UI_TYPE_TEXT, ut);

    // set the UI
    scene_set_ui_root(s, root);

    // set the scene
    game_set_scene(g, s);
    
    // speed
    float camSpeed = 1.7; // 1.7 rad a second
    float moveSpeed = 5.0; // 5 meters a second
    
    // update
    while (game_still_running(g)) {

        // dt
        float dt = get_dt();

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
        
        // update the game
        game_update(g);
    }

    // end
    game_destroy(g);
}