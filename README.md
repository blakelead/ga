# ga

playing with genetic algorithms to solve a 2d maze.

![demo](./docs/assets/demo.gif)

## description

this project uses a genetic algorithm to solve a 2d maze.
the maze is generated at runtime and the genetic algorithm is run to find the optimal path through the maze.
the project uses raylib library for the visual representation of the maze and the algorithm work.

the current implementation of the genetic algorithm is quite basic and not well optimized. solving the maze may take a long time especially for larger mazes and can even fail to do so. i plan on working on that once i gain more knowlege about this subject.

## build and run

the build requires `cmake`.

```bash
cmake -D CMAKE_BUILD_TYPE=Debug -S . -B build/Debug
cmake --build build/Debug
./build/Debug/ga
```

## configuration

the parameters of the project can be configured in the `parameters.toml` file.

## sources

the following resources helped me a lot:

- https://blog.gamedev.tv/procedural-generation-of-mazes-in-unreal-engine-part-i
- https://github.com/fadcrep/the-best-artificial-intelligence-books/blob/master/books/AI%20Techniques%20for%20Game%20Programming.pdf
