
# Microscript Scripting Language

## Language syntax
This language reads left to right; function/commands with varying amounts of
arguents will greedily consume tokens until they can't anymore. You must use
newlines or parentheses to get around this.

Comments start with `#`.

The arithmetic operators `+`, `-`, `*`, and `/` exist.
The boolean operators `+`, `|`, `=`, `<`, and `>` exist.

Refer below for a list of commands and events.

## Commands

### `as`
- Usage: `as <object> ...`
- Returns: `the object`
- Run the following command in the context of a different piece of data.
- You can run commands as:
  - Entities
  - Scenes
  - Mesh components
  - Velocity components
  - Collider components
  - All vectors
- Examples:
```
let myentity new entity

as myentity set position 10 20 30

# OR, using a do block allows for many commands:
as myentity do
    set position 10 20 30
    set rotation 0 1.57 0
end
```

### `attach`
- Usage: `attach <component>`
- Returns: `attached component`
- Attach a component to the current entity. Must be run in the context of an entity.
- Examples:
```
# Assume `my_mesh` is a mesh resource.

new "entity" with

    # simple velocity attached
    attach new "velocity"

    # collider; you can set properties on the new instance
    attach new "collider" with
        set size 1 2 3
    end

    # mesh; you can load it from a resource and/or set properties
    attach new "mesh" from my_mesh with
        set color 255 0 255
    end
end
```

### `call`
- Usage: `call <name> (with <args>)?`
- Returns: `varies`
- Call a function, either a engine-defined one or a scripter-defined one.
- If the function takes many parameters, it is recommended to comma-seperate them to prevent issues.
- Examples:
```

# simple function with no parameters
on say_hi do
    echo "hi"
end
call say_hi # hi

# With parameters
on say_name with name do
    echo "hello, " name "!"
end
call say_name "Nixii" # hello, Nixii!
```

### `despawn`
- Usage: `despawn <entity>`
- Returns: `nil`
- Despawn an entity and delete all its components.
- **NOTE:** this does NOT set variables to `nil`.
- Examples:
```
# destroy the entity
let e1 new "entity"
despawn e1

# destroys the collider too
let e1 new "entity" with
    attach new "collider"
end
despawn e1
```

### `do`
- Usage: `do ... end`
- Returns: `nil`
- Create a new scope and run the commands. Each line should have its own command; this can be buggy if you put it in one line.
- Examples:
```
let x 10
echo x # 10
do
    let x 20
    echo x # 20
    let y 30
    echo y # 30
end
echo x #20
echo y # error: no variable `y`
```

### `echo`
- Usage: `echo <args>`
- Returns: `string`
- Output all the args one after another. Appends them to each other; does not add spaces.
- Examples:
```
echo "Hello, world!"
# $  Hello, world!
let pos 1 2 3
echo "My position is" pos
# $  My position is1 2 3

# (there is no space on purpose to show that)
```

### `get`
- Usage: `get <name>`
- Returns: `varies`
- Get a property on the current context.
- Examples:
```
as some_entity do
    let pos get position
    echo "pos: " pos "!"
end
```

### `if`
- Usage: `if <condition> ... (else ...)?`
- Returns: `return of the if condition if true, return of else if false, else nil`
- Run the first `...` if the `condition` is true. If there is an `else` block, run the `else`'s `...`
- Examples:
```
let x 10
let y 20
let z 30

if x = y echo "x is equal to y"
# no output

if z > y do
    echo "z is > than y."
end else do
    echo "never mind!"
end
# $  z is > than y.
```

### `let`
- Usage: `let <name> <value>`
- Returns: `value`
- Set the variable `name` equal to the `value`, and create a variable `name` if it doesn't exist.
- Prioritizes reassigning rather than creating new variables.
- Examples:
```
let x 10
let y 30
echo x " is x, and y is " y
# $  10 is x, and y is 30

do
    let x 30
    echo x
    # $  30
    let z 5.5
    echo z
    # $  5.5
end
echo x
# $  30
echo z
# error: no variable `z`
```

### `load_mesh`
- Usage: `load_mesh <path>`
- Returns: `mesh_resource`
- Load a .obj file mesh into a resource, to be used for components
- Examples:
```
let my_mesh load_mesh "assets/test.obj"
new "entity" with
    new "mesh" from my_mesh
end
```

### `new`
- Usage: `new <typename> (from <object>)? (with ...)?`
- Returns: `a new instance of <typename>`
- Creates a new instance of `typename`, optionally from an object.
- If you put "with", there will be some code run in the context of this new entity before it is returned.
- Examples:
```
let myEntity new "entity" with
    attach new "collider"

    attach new "velocity" with
        set velocity 2 2 2
        echo "velocity attached"
    end

    # you can have from and/or with. Only some types have a "from" clause.
    attach new "mesh" from some_loaded_mesh with
        set color 255 0 255
    end
end
```

### `on`
- Usage: `on <name> (with <argnames>)? ...`
- Returns: `pointer to event <name>.`
- Create a new event that can be `call`ed. You can add argumetns with `argnames`.
- Examples:
```
on sayhi with name, extramessage do
    echo "Hi, " name "! You have a message: " extramessage
end

call sayhi "Nixii", "how long did this engine take to code?"
# $  Hi, Nixii! You have a message: how long did this engine take to code?
```

### `set`
- Usage: `set <name> <value>`
- Returns: `<value>`
- Set a property on the current context object to value.
- It can only set existing properties.
- Examples:
```
new "entity" with
    set position 1 2 3
    set rotation 4 5 6
    set scale 7 8 9
end

set light_direction 1 0 0
```

## Built-in Events

### `cos`
- Parameters:
  - `theta: <number>`
    - The angle you wnat the cosine of, in **RADIANS**.
- Returns: `number`
- Return the cosine of `theta`.
- Example: `echo call cos 3.1415 # (roughly) -1`

### `disable_mouse`
- Parameters: `none`
- Returns: `nil`
- Prevent the mouse from moving. `get_mouse_delta` still returns mouse deltas correctly.
- Example: `call disable_mouse`

### `enable_mouse`
- Parameters: `none`
- Returns: `nil`
- Allow the mouse to move again.
- Example: `call enable_mouse`

### `get_mouse_delta`
- Parameters: `none`
- Returns: `vec2`
- Return the pixel distance the mouse has travelled this frame.
- Example: `echo call get_mouse_delta # maybe 0 0 or -30 15`

### `is_key_down`
- Parameters:
  - `key <string>`
    - A string for the key pressed. Lowercase for letters. Uppercase for special.
- Returns: `boolean`
- Return true if a key is currently being held down and false otherwise.
- Example: `if (call is_key_down "a") echo "a is down!"`

### `is_mouse_enabled`
- Parameters: `none`
- Returns: `boolean`
- Check if the mouse can be moved.
- Example:
```
call disable_mouse
echo call is_mouse_enabled
# $  false
call enable_mouse
echo call is_mouse_enabled
# $  true
```

### `length`
- Parameters:
  - `vec: <vec2, vec3, or vec4>`
- Returns: `number`
- Return the magnitude of the supplied vector.
- Example: `echo call length 1 1 1 # (roughly) 1.73`

### `sin`
- Parameters:
  - `theta: <number>`
    - The angle you wnat the sine of, in **RADIANS**.
- Returns: `number`
- Return the sine of `theta`.
- Example: `echo call sin 1.57 # (roughly) 1`

### `sqrt`
- Parameters:
  - `value: <number>`
- Returns: `number`
- Return the square root of `value`.
- Example: `echo call sqrt 4 # 2`

### `square`
- Parameters:
  - `value: <number>`
- Returns: `number`
- Return the square of `value`.
- Example: `echo call square 4 # 16`


## Built-in Callbacks

### `frame`
- Parameters: 
  - `deltatime: <number>`
    - The deltatime since the last frame.
    - Great for making framerate-independent motion.
- This is called before each frame renders, before the physics simulation.
- Examples:
```
# the amount of time since the beginning of this session
let totalTime 0
on frame with dt do
    let totalTime totalTime + dt
    echo totalTime
end
```