
# microscript

## Commands

### `as`
Usage: `as <entity|scene> <command>`

- Run `command` as the selected entity or scene.
- `me` and `scene` will be set. If you are running as a scene,
- `me` will be set to `none`.

Example:
```
let e1 entity new
as e1 set position 0 3 0
```

<br>

### `end`
Usage: `end`

- Ends a previous block statement.
Example:
```
let e1 entity new with
    set position 10 10 10
    set rotation 45 0 45
end
```

<br>

### `set`
Usage: `set <property> <value>`

- Sets a certain property on `me` to be a certain `value`.
- Example: `set position 0 0 0`
- Example: `set guid Player`

<br>