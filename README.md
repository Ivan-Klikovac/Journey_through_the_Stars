# Journey through the Stars
A 2D ASCII roguelike in C built with `ncurses`. Space exploration themed, still early in development.

Based on the tutorial at https://dev.to/ignaoya/the-c-roguelike-tutorial-part-0-the-setup-1pfo. That tutorial is unfinished so I picked up where it left off.

It includes:
- primitive procedural generation
- simple combat system (speed, armor, damage based)
- logging system
- inventory system
- pathfinding system
- dynamic fov
- saving system (in progress)
- ... and more.

## Installation
For Windows: clone the repo, install `pdcurses` and run `compile.bat` or `debug.bat`.

For GNU/Linux: clone, install `ncurses` (likely present on your system), and run:

`gcc *.c -Iinclude -lncurses -o jtts`
