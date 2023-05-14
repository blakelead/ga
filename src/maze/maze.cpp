#include "maze/maze.hpp"
#include "maze/position.hpp"
#include <cmath>
#include <unordered_set>

maze::maze(int rows, int cols)
    : m_rows(rows), m_cols(cols),
      m_data(rows, std::vector<maze_cell>(cols, maze_cell::WALL))
{
}

// this method takes a path as input and returns a fitness value for that path.
// the method goes through each direction in the path, checks if the direction leads to a wall or has been visited before, and increments a penalty in such cases.
// the fitness value is calculated as the inverse of the distance from the last position to the exit, plus the penalty.
float maze::test_path(const path &path) const
{
  float penalty = 0;
  position current(m_entrance);
  std::unordered_set<int> visited;

  // TODO: add to global parameters
  float visited_weight = 0;
  float wall_weight = 0;
  float distance_weight = 1;

  for (const auto &direction : path.directions)
  {
    position next = current + direction;
    if (is_path(next))
    {
      if (visited.find(next.row * m_cols + next.col) != visited.end())
        penalty += visited_weight;
      else
        visited.insert(next.row * m_cols + next.col);
      current = next;
    }
    else if (is_wall(next))
    {
      penalty += wall_weight;
    }
    else if (is_exit(next))
    {
      current = next;
      break;
    }
  }

  float distance = distance_weight * std::sqrt(std::pow(current.row - m_exit.row, 2) + std::pow(current.col - m_exit.col, 2));
  return 1.0f / (distance + penalty + 1);
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

bool maze::is_entrance(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::ENTRANCE;
}

bool maze::is_exit(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::EXIT;
}

void maze::draw(int window_width, int window_height, int offset_x, int offset_y) const
{
  // calculate cell size from window and maze sizes
  int cell_width = window_width / m_cols;
  int cell_height = window_height / m_rows;
  int cell_size = std::min(cell_width, cell_height);

  // calculate the starting coordinate of the maze
  int maze_x = (window_width - m_cols * cell_size) / 2 + offset_x;
  int maze_y = (window_height - m_rows * cell_size) / 2 + offset_y;

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
      DrawRectangle(col * cell_size + maze_x,
                    row * cell_size + maze_y,
                    cell_size, cell_size, color);
    }
  }
}

void maze::draw_path(int window_width, int window_height, int offset_x, int offset_y, const path &path, Color color)
{
  // calculate cell size from window and maze sizes
  int cell_width = window_width / m_cols;
  int cell_height = window_height / m_rows;
  int cell_size = std::min(cell_width, cell_height);
  float line_size = std::max(1, cell_size / 8);

  // calculate the starting coordinate of the maze
  int maze_x = (window_width - m_cols * cell_size) / 2 + offset_x;
  int maze_y = (window_height - m_rows * cell_size) / 2 + offset_y;

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
