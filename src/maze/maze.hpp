#pragma once

#include "maze/path.hpp"

enum class maze_cell
{
  PATH,
  WALL,
  START,
  END
};

class maze
{
public:
  maze(int rows, int cols);
  float test_path(const path &path) const;
  bool is_within_bounds(const position &pos, int row_offset = 0, int col_offset = 0) const;
  void set_path(const position &pos);
  void set_wall(const position &pos);
  void set_start(const position &pos);
  void set_end(const position &pos);
  bool is_path(const position &pos) const;
  bool is_wall(const position &pos) const;
  bool is_start(const position &pos) const;
  bool is_end(const position &pos) const;
  void draw(int window_width, int window_height) const;
  void draw_path(int window_width, int window_height, const path &path);
  void print() const;

private:
  std::vector<std::vector<maze_cell>> m_data;
  int m_rows;
  int m_cols;
  position m_start;
  position m_end;
};
