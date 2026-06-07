
<div align="center">
    <img src="docs_images/microgame full title 4x.png">
</div>

## What is microgame?
Microgame is a small game engine for retro-esque video games.<br>
It will be usable as a custom coding language *and* as a static C library.

## Features
- An ECS scene graph allowing for easy addition of new data
- Collision & velocity built-in
- A small custom coding language for use in the engine

## Planned Features
- ~~A full editor for the engine, coded using itself~~ **NOTE:** this feature was cut due to time constraints. It may be added at a later date

## How to build?
1) Make sure you have Raylib installed.
2) Run `make engine`. This creates a `./microgame` executable.
3) Run the `./microgame` executable. This will run `main.microscript` in the same directory.

## Langauge docs
Found [here (MICROSCRIPT.md)](MICROSCRIPT.md).

## Platform Support
> ✅ works<br>
> ❗️ untested<br>
> ❌ unsupported

|  Version  |  Distribution  |  MacOS (arm64)  |  MacOS (x86)  |  Linux (arm64)  |  Linux (x86)  |  Windows  |
|:---------:|:--------------:|:---------------:|:-------------:|:---------------:|:-------------:|:---------:|
| dev branch | library | ✅ | ❗️ | ❗️ | ❗️ | ❗️ |
|  | scripting language | ✅ | ❗️ | ❗️ | ❗️ | ❗️ |