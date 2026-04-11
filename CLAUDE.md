# CLAUDE.md

This project is a C++ and raylib platformer/block sandbox game.

## Architecture

The game uses a **screen-based state machine** with an **ECS (Entity-Component-System)** for the gameplay screen.

### Screen System
- `raylib_game.c` — entry point; owns the main loop, global assets (`font`, `music`, `fxCoin`), and screen transition logic (fade in/out via `transAlpha`)
- `screens.hpp` — declares `GameScreen` enum (`LOGO`, `TITLE`, `OPTIONS`, `GAMEPLAY`, `ENDING`) and the five lifecycle functions each screen must implement: `Init`, `Update`, `Draw`, `Unload`, `Finish`
- `screen_*.c/.cpp` — one file per screen implementing those five functions
- Screen files are written in C except `screen_gameplay.cpp` which uses C++ for ECS

### ECS (`src/ecs/`)
Defined in `Entity.hpp` (everything lives in the `ecs` namespace):

- **`Component`** — base class; each subclass has a static `ComponentType` tag. Current components: `TransformComponent` (position + velocity), `BoundingBoxComponent` (size + offset), `TextureComponent` (path + size), `PlayerComponent`, `PhysicsComponent` (isGrounded)
- **`Entity`** — holds a `map<ComponentType, AnyComponent>`; access via `entity->GetComponent<T>()`
- **`World`** — owns all entities and systems; drives `Start()` / `Update()` / `Render()` / `ProcessEvents()`; emits events into an event queue
- **`System`** — base class with `OnStart`, `ProcessEvent`, `Update`, `Render` hooks
- Current systems: `RenderSystem`, `PlayerSystem`, `PhysicsSystem`

**Querying entities** — two view methods on `World`, both backed by a bitmask view cache:
- `GetEntities<Ts...>()` — iterates entities that have all of `Ts`, yielding `std::tuple<Ts*...>` (component pointers only)
- `GetComponentsWithEntity<Ts...>()` — same filter, but yields `std::tuple<Entity*, Ts*...>`; use this when `Entity*` is needed (e.g. to emit events)

**PhysicsSystem** applies gravity and resolves AABB collisions by pushing entities back using their pre-move position. **PlayerSystem** reads WASD and moves the player entity. **RenderSystem** draws `TextureComponent` at `TransformComponent.position`, caching loaded images/textures in maps.

### Mixed C/C++
The project mixes `.c` (screen files, main) and `.cpp` (gameplay screen, ECS). `screens.hpp` wraps all screen function declarations in `extern "C"` to prevent C++ name mangling when called from `raylib_game.c`.

## Coding Conventions

From `CONVENTIONS.md`:
- Always use curly brackets for if statements
- Do not insert spaces to align up "=" or comments
- Functions: `TitleCase` — `InitGameplayScreen()`
- Variables/struct members: `lowerCase`
- Do not remove existing comments irrelevant to the changes you make
- Defines/macros/enum members: `ALL_CAPS`
- Structs/enums/typedefs: `TitleCase`
- Float literals always `x.xf` — `10.0f`
- 4 spaces, no tabs; no trailing spaces
- All variables always initialized at declaration
- Braces open/close on their own aligned lines
- Control flow keywords followed by a space: `if (condition)`
- Compound conditions always parenthesized: `if ((a > 1) && (b < 50) && flag)`
- Files/directories named `snake_case`
- Resources organized under `resources/` by category (audio/fx, audio/music, screens/\<name\>, characters, common)