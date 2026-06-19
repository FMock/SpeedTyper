# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Kaboom Typer (formerly SpeedTyper) — a Windows desktop typing game written in C++ using SDL2 + OpenGL (immediate mode) for windowing/rendering and FMOD for audio. The player types falling words to destroy them before they stack to the top of the play field. Author: Frank Mock. Project site: http://www.frankmock.com/software/kaboomtyper/index.php

## Build & run

This is a **Visual Studio** project on Windows. The solution/project files (`*.sln`, `*.vcxproj`, `*.filters`, `*.user`) and `Debug`/`Release` output dirs are intentionally **git-ignored** (see `.gitignore`), so they are not in the repo — build by opening the local Visual Studio solution and compiling, or run the prebuilt `kaboom_typer_v02/KaboomTyper.exe`.

- **External dependencies** (must be available to the compiler/linker): SDL2, GLEW, FMOD (`fmod.hpp`), and OpenGL. Headers are included as `<SDL.h>`, `<GL/glew.h>`, `<fmod.hpp>`.
- **Runtime DLLs** live in the repo root and must sit next to the executable: `SDL2.dll`, `glew32.dll`, `fmod.dll`, `fmodL.dll`.
- **Working-directory-relative assets**: the exe loads `words.txt`, `high.txt`, `images/*.tga`, and `sounds/*` by relative path, so it must be launched from a directory containing those. `kaboom_typer_v02/` is the packaged distribution copy (its own `words.txt`/`high.txt` + DLLs + exe).
- There is **no test suite, linter, or CI** — verification is by running the game.

## Data files

- `words.txt` — the word list, grouped by category header lines: `#ANIMALS`, `#PLACES`, `#THINGS`, `#TRANSPORTATION`, `#MUSIC`, `#ANIME`. At load (`readLines` in `main.cpp`) each header's position is recorded as the start index of that category in the `words` vector; category ranges are computed as the gap between consecutive header indices. **The header lines are themselves pushed into `words`**, so word selection logic depends on these markers staying in order. Adding a category means adding a header constant + index variable + a branch in both `readLines` and the block-spawn category switch.
- `high.txt` — two lines: line 1 is the integer high score, line 2 is the 3-character initials. Read by `readHighScore`, written by `writeHighScore`.

## Architecture

The game is a single-threaded loop. **`main.cpp` owns everything**: it holds all global state (game objects, vectors of blocks/hits, FMOD handles, the `stringToImageMap`), runs the one `while (!shouldExit)` loop, and contains input handling, update, collision, scoring, and draw phases inline. The other classes are mostly passive data+behavior objects that `main` drives. Expect to edit `main.cpp` for almost any gameplay change.

Key cross-cutting pieces:

- **`Game_Data` (`game_data.h`)** — a single shared struct holding all mutable game state (score, level, lives, current word, GUI flags, mouse, window/layout constants, `deltaTime`). Almost every object is constructed with a `Game_Data&` and keeps a pointer to it; this is the central blackboard objects read/write to coordinate. Layout/positioning is hard-coded as `static const int` constants here (window 800×600, HUD regions, button hit-boxes).

- **Bitmap-font rendering via `stringToImageMap`** — there is no font engine. `main` loads one TGA texture per glyph/color/digit into `std::map<std::string, GLuint> stringToImageMap`. `TextBlock` (the falling words) and `TextWriter` (the player's typed input line) each receive this map and render strings by drawing one textured quad per character. `DrawUtils` (`glTexImageTGAFile`, `glDrawSprite`) is the only low-level rendering helper; all drawing is OpenGL fixed-function immediate mode.

- **`Sprite` (`sprite.h`)** — base class for on-screen objects; subclasses are `TextBlock` (a falling word, has `text`, `moving`, `remove`, AABB collision box) and `Hit` (an explosion animation). Animations use `AnimationData` + `AnimFrameData` (start frame + frame count) advanced by `deltaTime`.

- **Collision** — axis-aligned bounding boxes (`AABB`) with `AABBIntersect` in `main.cpp`. Falling blocks stop when they intersect a lower block; stacking height drives the game-over check.

- **Input** — `KeyStates` tracks per-key press events for the typed-word buffer (`testing` string in `main`), while arrow keys / Escape are polled directly from SDL keyboard state in the loop. Special inputs: typing `KABOOM` destroys a word for a penalty; arrows control start/pause/reset.

- **`GUI` / `Selection`** — start-screen UI: word-category buttons and the music toggle, hit-tested against the button-rectangle constants in `Game_Data`. `Selection` is one clickable menu item.

- **`Hud`** — draws score / high score / initials / word counts, again by composing glyph textures from `stringToImageMap`.

- **`Stats`** — score/total/correct counters; mutating these goes through `Stats` (`increaseScore`, etc.), which writes back into `Game_Data`.

- **`Timer` (`timer.h`)** — second-resolution counter built on `SDL_GetTicks()` and `deltaTime`; `main` uses it to spawn a new block every `blockInterval` seconds.

## Conventions to match

- Most classes expose a `to_string()` for debugging; the loop has commented-out `printf(...->to_string())` lines used as a poor-man's debugger. Follow that pattern rather than adding a logging framework.
- Headers use both `#pragma once` and include guards; small structs define inline constructors in the header (note the legacy `ClassName::method` qualified-inline style inside class bodies).
- Game tuning lives as named constants in `game_data.h` / top-of-`main.cpp` globals, not config files.
