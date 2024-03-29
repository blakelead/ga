#pragma once

#include "maze/path.hpp"
#include <raylib.h>

enum class maze_cell
{
  PATH,
  WALL,
  ENTRANCE,
  EXIT
};

class maze
{
public:
  maze(int rows, int cols);
  void compute_grid_score();
  float test_path(const path &path) const;
  bool is_within_bounds(const position &pos, int row_offset = 0, int col_offset = 0) const;
  void set_path(const position &pos);
  void set_wall(const position &pos);
  void set_entrance(const position &pos);
  void set_exit(const position &pos);
  bool is_path(const position &pos) const;
  bool is_wall(const position &pos) const;
  bool is_dead_end(const position &pos) const;
  bool is_entrance(const position &pos) const;
  bool is_exit(const position &pos) const;
  void draw(int width, int height, int offset_x, int offset_y) const;
  void draw_path(int width, int height, int offset_x, int offset_y, const path &path, Color color);
  void draw_all_paths(int width, int height, int offset_x, int offset_y, std::vector<path> all_paths, Color color);

private:
  std::vector<std::vector<maze_cell>> m_data;
  std::vector<std::vector<int>> m_grid_score;
  int m_rows;
  int m_cols;
  position m_entrance;
  position m_exit;
};
