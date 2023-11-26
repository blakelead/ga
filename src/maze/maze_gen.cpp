#include "maze_gen.hpp"
#include <cmath>
#include <random>
#include <stack>

maze_gen::maze_gen() {}

maze maze_gen::generate(int rows, int cols, maze_type type)
{
  maze maze(rows, cols);
  switch (type)
  {
  case maze_type::DFS:
    dfs(maze);
    break;
  case maze_type::PRIM:
    prim(maze);
    break;
  }
  maze.set_entrance({1, 0});
  maze.set_exit({rows - 2, cols - 1});
  return maze;
}

void maze_gen::dfs(maze &maze)
{
  std::stack<position> visited;

  position start(1, 1);
  maze.set_path(start);
  visited.push(start);

  std::vector<position> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  while (!visited.empty())
  {
    position current = visited.top();
    visited.pop();

    std::shuffle(directions.begin(), directions.end(), std::default_random_engine(rand()));
    for (const position &direction : directions)
    {
      position next = current + direction;
      position next_next = current + direction * 2;
      if (maze.is_wall(next_next))
      {
        maze.set_path(next);
        maze.set_path(next_next);
        visited.push(next_next);
      }
    }
  }
}

void maze_gen::prim(maze &maze)
{
  std::vector<position> frontier;

  position start(1, 1);
  maze.set_path(start);

  std::vector<position> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  for (const position &direction : directions)
  {
    position next = start + direction * 2;
    if (maze.is_within_bounds(next, 1, 1))
      frontier.push_back(next);
  }

  while (!frontier.empty())
  {
    // get a random position from frontier vector
    int rand_index = rand() % frontier.size();
    position current = frontier[rand_index];
    frontier.erase(frontier.begin() + rand_index);

    std::shuffle(directions.begin(), directions.end(), std::default_random_engine(rand()));
    for (const position &direction : directions)
    {
      position next = current + direction * 2;
      if (maze.is_path(next))
      {
        position neighbor((current.row + next.row) / 2, (current.col + next.col) / 2);
        maze.set_path(current);
        maze.set_path(neighbor);

        int path_neighbors = 0;
        for (const position &direction : directions)
        {
          position neighbor = current + direction;
          if (maze.is_path(neighbor))
            ++path_neighbors;
        }
        if (path_neighbors == 1)
        {
          for (const position &direction : directions)
          {
            position neighbor = current + direction * 2;
            if (maze.is_wall(neighbor))
              frontier.push_back(neighbor);
          }
        }
        break;
      }
    }
  }
}
