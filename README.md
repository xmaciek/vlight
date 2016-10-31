# VLight
Flight simulator over comupter generated landscape

### About:
This is my master degree diploma work, defended with best mark in 2015.
In short this is flight simulator over computer generated ladnscape.

In details my main focus here was to design an algorithm which can generate terrain landscapes on multiple threads.
The flight simulator and physics behind it was just an addition to project.

### Usage:
#### Compilation:
Please start shell script `compile.sh`. The compilation may take a while.
After that please start `./mgr`.

#### Parameters:
`--threads #` - Number of threads used for landscape generation.
`--width #` - Window width.
`--height #` - Window height.
`--density #` - Number of vertices in width/height of grid representing landscape. Total amount of vertices is equal to #^2.
`--seed #` - Seed value for random number generation.

### Requirments:
Linux, SDL 1.2, GLEW, OpenGL 3.3 graphics card.

###### License GNU GPLv3
