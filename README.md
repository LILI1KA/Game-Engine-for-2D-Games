# C++ Game Engine and Terminal Games

A reusable object-oriented 2D game engine written in C++14, together with two
terminal games built on top of it: a two-level Breakout game and a three-level
shooter RPG. Rendering and keyboard input use ncurses.

## Highlights

- Models points, rectangles, and composite bitmap objects through a shared
  polymorphic `Object` interface.
- Implements straight-line, gravitational, cyclic, and player-controlled
  movement as attachable observer components.
- Supports configurable object-pair collision responses, including pass,
  bounce, stop, damage, win, and lose states.
- Runs movement, collision detection, object removal, boundary handling,
  rendering, and frame timing through a central game loop.
- Separates the model, view, controller, object behavior, and game-specific
  level construction so the same engine can support different games.
- Uses smart pointers and observer/subject relationships to manage shared game
  objects and behavior components.

## Included Games

### Breakout

A two-level brick-breaking game with a player-controlled paddle, a bouncing
ball, destructible brick layouts, health tracking, level transitions, restart,
and win/lose states.

### Shooter RPG

A three-level terminal action game with directional movement and shooting,
animated hazards, enemies, health pickups, projectile interactions, exits, and
a multi-state final boss encounter.

Controls:

- Arrow keys: move the player or paddle.
- `W`, `A`, `S`, `D`: shoot up, left, down, and right in the shooter RPG.
- Follow the on-screen prompt to restart, advance, or quit after a level ends.

## Architecture

```text
Game-specific clients
  |-- break.cc + break_client.*       (Breakout)
  `-- arlg.cc + arlg_client.*         (Shooter RPG)
             |
             v
Model / game loop
  |-- movement observers
  |-- pairwise collision policies
  |-- object lifecycle and end states
  |-- keyboard controller
  `-- ncurses view, border, and status display
             |
             v
Object hierarchy
  |-- SingleChar
  |-- Rectangle
  `-- Bitmap (composite characters)
```

The generic engine is shared by both executables. Each game-specific client
constructs its own objects, collision relationships, levels, and rules.

## Repository Structure

```text
Game-Engine-and-2D-Games/
|-- src/
|   |-- object.*             # Object hierarchy and collision geometry
|   |-- movement.*           # Reusable movement behaviors
|   |-- collide.*            # Collision policy state
|   |-- model.*              # Update loop and object lifecycle
|   |-- view.*, status.*     # ncurses rendering and status display
|   |-- controller.*         # Input abstraction
|   |-- cursekeyboard.*      # ncurses keyboard implementation
|   |-- break*               # Breakout entry point and game client
|   `-- arlg*                # Shooter RPG entry point and game client
|-- Makefile
`-- README.md
```

Compiled objects, dependency files, executables, ZIP archives, duplicate source
snapshots, and unrelated course materials are intentionally excluded.

## Build

Requirements:

- GNU Make;
- a C++14-compatible compiler such as `g++`;
- ncurses development headers and library.

On Ubuntu or Debian, install the native dependency with:

```bash
sudo apt-get install build-essential libncurses-dev
```

Build both games:

```bash
make
```

The executables are written to:

```text
bin/breakout
bin/shooter_rpg
```

Individual targets are also available:

```bash
make breakout
make shooter_rpg
```

Remove all generated build files with:

```bash
make clean
```

## Run

The games require a terminal large enough for the fixed ncurses play area
(approximately 80 columns by 24 rows).

```bash
./bin/breakout
./bin/shooter_rpg
```

An optional starting level can be supplied:

```bash
./bin/breakout 2
./bin/shooter_rpg 3
```

## Notes

- The implementation preserves the original source structure and naming. The
  `arlg` source prefix is retained for the shooter RPG while the generated
  executable uses the clearer name `shooter_rpg`.
- The renderer targets a Unix-like ncurses environment. On Windows, build and
  run it through WSL or another environment that provides ncurses.

