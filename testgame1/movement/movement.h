
#include <microgame/microgame.h>

#define MOVE_SPEED 1.0
#define JUMP_POWER 4.0
#define CAM_SPEED 0.5
#define GRAVITY 8.0

#define GROUND_FRICTION 1.5
#define AIR_FRICTION 1.2

#define DASH_SPEED 20.0
#define DASH_FRICTION 1.1
#define DASH_COOLDOWN 2.0

void update_movement(scene *s, entity p);