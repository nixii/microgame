
# microcript

## About
Microscript is the scripting language the engine uses.

## Global variables
* `self: *entity` (the current entity)
* `here: *scene` (the current scene)


## Functions
All of these are global and accessible everywhere. Yes, some can take many types.

### Entity
 * `get_entity(name: string): *entity`

 * `get_first_child(parent: *entity): *entity`
 * `get_child(parent: *entity, name: string): *entity`
 * `add_child(parent: *entity, child: *entity)`
 * `set_parent(child: *entity, parent: *entity)`
 * `remove_child(parent: *entity, child: *entity): bool`
 * `remove_parent(child: *entity, parent: *entity): bool`

 * `get_transform(entity: *entity): transform`
 * `set_transform(entity: *entity, transf: transform)`
 * `get_global_transform(entity: *entity): transform`

 * `get_pos(entity: *entity): vec3`
 * `set_pos(entity: *entity, pos: vec3)`
 * `set_x(entity: *entity, x: number)`
 * `set_y(entity: *entity, y: number)`
 * `set_z(entity: *entity, z: number)`

 * `get_rot(entity: *entity): vec3`
 * `set_rot(entity: *entity, rot: vec3)`
 * `set_rot_x(entity: *entity, x: number)`
 * `set_rot_y(entity: *entity, y: number)`
 * `set_rot_z(entity: *entity, z: number)`

 * `get_scale(entity: *entity): vec3`
 * `set_scale(entity: *entity, scale: vec3)`
 * `set_scale_x(entity: *entity, x: number)`
 * `set_scale_y(entity: *entity, y: number)`
 * `set_scale_z(entity: *entity, z: number)`

 * `has_mesh(entity: *entity): bool`
 * `get_mesh(entity: *entity): *mesh`
 * `attach_mesh(entity: *entity, mesh: mesh)`

 * `has_collider(entity: *entity): bool`
 * `get_collider(entity: *entity): *collider`
 * `attach_collider(entity: *entity, collider: collider)`

 * `has_velocity(entity: *entity): bool`
 * `get_velocity(entity: *entity): *velocity`
 * `attach_velocity(entity: *entity, velocity: velocity)`

### Scene
 * `spawn(): *entity`
 * `despawn(entity: *entity)`

 * `load_scene(scene: *scene)`
 * `go_to_scene(scene: *scene)`

### Camera
 * `get_transform(cam: *camera): transform`
 * `set_transform(cam: *camera, transf: transform)`

 * `get_pos(cam: *camera): vec3`
 * `set_pos(cam: *camera, pos: vec3)`
 * `set_x(cam: *camera, x: number)`
 * `set_y(cam: *camera, y: number)`
 * `set_z(cam: *camera, z: number)`

 * `get_rot(cam: *camera): vec3`
 * `set_rot(cam: *camera, rot: vec3)`
 * `set_rot_x(cam: *camera, x: number)`
 * `set_rot_y(cam: *camera, y: number)`
 * `set_rot_z(cam: *camera, z: number)`