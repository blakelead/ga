#include "maze/maze.hpp"
#include "maze.hpp"
#include "maze/position.hpp"
#include <cmath>
#include <queue>
#include <unordered_set>

maze::maze(int rows, int cols)
    : m_rows(rows), m_cols(cols),
      m_data(rows, std::vector<maze_cell>(cols, maze_cell::WALL)),
      m_grid_score(rows, std::vector<int>(cols, INT_MAX))
{
}

void maze::compute_grid_score()
{
  m_grid_score[m_exit.row][m_exit.col] = 0;
  std::queue<position> queue;
  queue.push(m_exit);
  std::vector<position> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // UP, RIGHT, DOWN, LEFT
  while (!queue.empty())
  {
    position current = queue.front();
    queue.pop();
    for (const auto &direction : directions)
    {
      position next = current + direction;
      if (is_path(next) && m_grid_score[current.row][current.col] + 1 < m_grid_score[next.row][next.col])
      {
        m_grid_score[next.row][next.col] = m_grid_score[current.row][current.col] + 1;
        queue.push(next);
      }
    }
  }
}

// this method takes a path as input and returns a fitness value for that path.
// the method goes through each direction in the path, checks if the direction leads to a wall or has been visited before, and increments a penalty in such cases.
// the fitness value is calculated as the inverse of the distance from the last position to the exit, plus the penalty.
float maze::test_path(const path &path) const
{
  position current(m_entrance);

  for (const auto &direction : path.directions)
  {
    position next = current + direction;
    if (is_path(next))
    {
      current = next;
    }
    else if (is_exit(next))
    {
      current = next;
      break;
    }
  }
  int distance_to_exit = m_grid_score[current.row][current.col];
  return 1.0f / (distance_to_exit + 1.0f);
}

// this method checks if a given position (with an optional offset) is within the maze boundaries.
// the optional offset is useful for excluding the outer walls in certain cases.
bool maze::is_within_bounds(const position &pos, int row_offset, int col_offset) const
{
  return pos.row >= row_offset && pos.row < m_rows - row_offset &&
         pos.col >= col_offset && pos.col < m_cols - col_offset;
}

void maze::set_path(const position &pos)
{
  m_data[pos.row][pos.col] = maze_cell::PATH;
}

void maze::set_wall(const position &pos)
{
  m_data[pos.row][pos.col] = maze_cell::WALL;
}

void maze::set_entrance(const position &pos)
{
  m_data[pos.row][pos.col] = maze_cell::ENTRANCE;
  m_entrance.row = pos.row;
  m_entrance.col = pos.col;
}

void maze::set_exit(const position &pos)
{
  m_data[pos.row][pos.col] = maze_cell::EXIT;
  m_exit.row = pos.row;
  m_exit.col = pos.col;
}

bool maze::is_path(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::PATH;
}

bool maze::is_wall(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::WALL;
}

bool maze::is_dead_end(const position &pos) const
{
  int dead_ends = 0;
  std::vector<position> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // UP, RIGHT, DOWN, LEFT
  for (const auto &direction : directions)
  {
    if (is_wall(pos + direction))
      dead_ends++;
  }
  return dead_ends == 3;
}

bool maze::is_entrance(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::ENTRANCE;
}

bool maze::is_exit(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::EXIT;
}

void maze::draw(int width, int height, int offset_x, int offset_y) const
{
  // calculate cell size from window and maze sizes
  int cell_width = width / m_cols;
  int cell_height = height / m_rows;
  int cell_size = std::min(cell_width, cell_height);

  // calculate the starting coordinate of the maze
  int maze_x = (width - m_cols * cell_size) / 2 + offset_x;
  int maze_y = (height - m_rows * cell_size) / 2 + offset_y;

  for (int row = 0; row < m_rows; row++)
  {
    for (int col = 0; col < m_cols; col++)
    {
      Color color;
      if (is_entrance({row, col}))
        color = ORANGE;
      else if (is_path({row, col}))
        color = RAYWHITE;
      else if (is_wall({row, col}))
        color = LIGHTGRAY;
      else if (is_exit({row, col}))
        color = GREEN;
      DrawRectangle(col * cell_size + maze_x, row * cell_size + maze_y, cell_size, cell_size, color);
    }
  }
}

void maze::draw_path(int width, int height, int offset_x, int offset_y, const path &path, Color color)
{
  // calculate cell size from window and maze sizes
  int cell_width = width / m_cols;
  int cell_height = height / m_rows;
  int cell_size = std::min(cell_width, cell_height);
  float line_size = std::max(1, cell_size / 8);

  // calculate the starting coordinate of the maze
  int maze_x = (width - m_cols * cell_size) / 2 + offset_x;
  int maze_y = (height - m_rows * cell_size) / 2 + offset_y;

  position current = m_entrance;
  for (const auto &direction : path.directions)
  {
    if (is_path(current + direction) || is_exit(current + direction))
    {
      position next = current + direction;
      DrawLineEx({current.col * cell_size + maze_x + (cell_size / 2.0f),
                  current.row * cell_size + maze_y + (cell_size / 2.0f)},
                 {next.col * cell_size + maze_x + (cell_size / 2.0f),
                  next.row * cell_size + maze_y + (cell_size / 2.0f)},
                 line_size, color);
      DrawCircle(current.col * cell_size + maze_x + (cell_size / 2.0f),
                 current.row * cell_size + maze_y + (cell_size / 2.0f),
                 line_size, color);
      DrawCircle(next.col * cell_size + maze_x + (cell_size / 2.0f),
                 next.row * cell_size + maze_y + (cell_size / 2.0f),
                 line_size, color);
      current = next;
    }
  }
}

void maze::draw_all_paths(int width, int height, int offset_x, int offset_y, std::vector<path> all_paths, Color color)
{
  for (const auto path : all_paths)
    draw_path(width, height, offset_x, offset_y, path, color);
}
