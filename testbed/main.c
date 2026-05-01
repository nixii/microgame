
#include "microgame/engine/components/mesh.h"
#include "microgame/engine/resource/image.h"
#include "microgame/engine/resource/mesh.h"
#include "microgame/engine/ui/container.h"
#include "microgame/util/color.h"
#include "microgame/util/math.h"
#include "microgame/microgame.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(void)
{

    // random seed
    srand(time(NULL));

    // create the game
    microgame *g = game_new(1600, 1000, 60, "hi world");
    scene *s = scene_new();

    // make the mesh resource
    mesh_resource mr = mesh_resource_from_obj("assets/test.obj");
    mesh m = mesh_from_resource(rgb_rand(), mr);

    // make image resource
    image_resource ir = image_resource_from("assets/overflow.png");

    // parent entity
    entity parent = scene_spawn(s);
    transform *parentTransform = get_transform(s, parent);
    parentTransform->pos.x = 2;

    // make another movement
    entity superParent = scene_spawn(s);
    transform *superTransform = get_transform(s, superParent);
    set_parent(s, parent, superParent);
    
    // make one test entity to move
    entity test = scene_spawn(s);
    scene_attach_mesh(s, test, m);
    transform *tT = get_transform(s, test);
    tT->pos.x = 4;
    set_parent(s, test, parent);

    // make one stationary test entity
    entity stationary = scene_spawn(s);
    mesh *m2 = scene_attach_mesh(s, stationary, m);
    m2->color = rgb(255, 0,  255);

    // get the transform
    camera *c = &s->camera;
    c->transform.pos = vec3_new(0, 3, -10);

    // create the UI
    ui_container *ui = ui_container_empty();
    ui->size = ui_vec_new(0.3, -10, 1, -10);
    ui->pos = ui_vec_new(0, 5, 0, 5);
    scene_set_ui_root(s, ui);

    // create an element for the image
    ui_container *imgContainer = ui_container_empty();
    imgContainer->size = ui_vec_new(1, 0, 1, 0);
    imgContainer->pos = ui_vec_new(1, 5, 0, 0);
    ui_container_add_child(imgContainer, ui);

    // add the image
    ui_image *imgui = ui_image_new(&ir);
    ui_container_bind_type(imgContainer, UI_TYPE_IMAGE, imgui);
    
    // set the scene
    game_set_scene(g, s);
    
    // speed
    float camSpeed = 1.7; // 1.7 rad a second
    float moveSpeed = 5.0; // 5 meters a second
    
    // update
    while (game_still_running(g)) {

        // get deltatime
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

        // rotate the test
        parentTransform->rot.z += 1 * dt;
        superTransform->rot.x += 1.25 * dt;
        tT->rot.y += 2 * dt;
        
        // update the game
        game_update(g);
    }
    
    // destroy resources
    mesh_resource_destroy(&mr);
    image_resource_destroy(&ir);

    // end
    game_destroy(g);
}