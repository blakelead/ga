#pragma once

#include "maze/maze.hpp"

enum class maze_type
{
  DFS,
  PRIM
};

class maze_gen
{
public:
  maze_gen();
  maze generate(int rows, int cols, maze_type type = maze_type::PRIM);

private:
  void dfs(maze &maze);
  void prim(maze &maze);
};
