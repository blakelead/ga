#include "maze/maze.hpp"
#include "maze/position.hpp"
#include "raylib.h"
#include "spdlog/spdlog.h"
#include <cmath>
#include <unordered_set>

maze::maze(int rows, int cols)
    : m_rows(rows), m_cols(cols),
      m_data(rows, std::vector<maze_cell>(cols, maze_cell::WALL))
{
}

float maze::test_path(const path &path) const
{
  int penalty = 0;
  position current_position(m_start);
  std::unordered_set<int> visited;

  for (const auto &direction : path.directions)
  {
    position next_position = current_position + direction;
    if (visited.find(next_position.row * m_cols + next_position.col) != visited.end())
    {
      // penalty++;
    }
    if (is_wall(next_position))
    {
      // penalty++;
    }
    else if (is_path(next_position))
    {
      current_position = next_position;
      visited.insert(current_position.row * m_cols + current_position.col);
    }
    else if (is_end(next_position))
    {
      current_position = next_position;
      break;
    }
  }

  float distance = std::sqrt(std::pow(current_position.row - m_end.row, 2) + std::pow(current_position.col - m_end.col, 2));
  return 1 / (distance * 8 + penalty + 1);
}

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

void maze::set_start(const position &pos)
{
  m_data[pos.row][pos.col] = maze_cell::START;
  m_start.row = pos.row;
  m_start.col = pos.col;
}

void maze::set_end(const position &pos)
{
  m_data[pos.row][pos.col] = maze_cell::END;
  m_end.row = pos.row;
  m_end.col = pos.col;
}

bool maze::is_path(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::PATH;
}

bool maze::is_wall(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::WALL;
}

bool maze::is_start(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::START;
}

bool maze::is_end(const position &pos) const
{
  return is_within_bounds(pos) && m_data[pos.row][pos.col] == maze_cell::END;
}

void maze::draw(int window_width, int window_height) const
{
  // calculate cell size from window and maze sizes
  int cell_width = window_width / m_cols;
  int cell_height = window_height / m_rows;
  int cell_size = std::min(cell_width, cell_height);

  // calculate the starting coordinate of the maze
  int maze_x = (window_width - m_cols * cell_size) / 2;
  int maze_y = (window_height - m_rows * cell_size) / 2;

  for (int row = 0; row < m_rows; row++)
  {
    for (int col = 0; col < m_cols; col++)
    {
      Color color;
      if (is_start({row, col}))
        color = ORANGE;
      else if (is_path({row, col}))
        color = RAYWHITE;
      else if (is_wall({row, col}))
        color = LIGHTGRAY;
      else if (is_end({row, col}))
        color = GREEN;
      DrawRectangle(col * cell_size + maze_x,
                    row * cell_size + maze_y,
                    cell_size, cell_size, color);
    }
  }
}

void maze::draw_path(int window_width, int window_height, const path &path)
{
  // calculate cell size from window and maze sizes
  int cell_width = window_width / m_cols;
  int cell_height = window_height / m_rows;
  int cell_size = std::min(cell_width, cell_height);

  // calculate the starting coordinate of the maze
  int maze_x = (window_width - m_cols * cell_size) / 2;
  int maze_y = (window_height - m_rows * cell_size) / 2;

  position current = m_start;
  for (const auto &direction : path.directions)
  {
    if (is_path(current + direction) || is_end(current + direction))
    {
      position next = current + direction;
      DrawLineEx({current.col * cell_size + maze_x + (cell_size / 2.0f),
                  current.row * cell_size + maze_y + (cell_size / 2.0f)},
                 {next.col * cell_size + maze_x + (cell_size / 2.0f),
                  next.row * cell_size + maze_y + (cell_size / 2.0f)},
                 3, RED);
      DrawCircle(current.col * cell_size + maze_x + (cell_size / 2.0f),
                 current.row * cell_size + maze_y + (cell_size / 2.0f),
                 3, RED);
      DrawCircle(next.col * cell_size + maze_x + (cell_size / 2.0f),
                 next.row * cell_size + maze_y + (cell_size / 2.0f),
                 3, RED);
      current = next;
    }
  }
}

void maze::print() const
{
  for (int row = 0; row < m_rows; row++)
  {
    for (int col = 0; col < m_cols; col++)
    {
      if (is_start({row, col}))
        printf("X");
      else if (is_path({row, col}))
        printf(" ");
      else if (is_wall({row, col}))
        printf("#");
      else if (is_end({row, col}))
        printf("O");
    }
    printf("\n");
  }
  printf("\n");
}
