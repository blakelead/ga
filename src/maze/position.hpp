#pragma once

struct position
{
  int row;
  int col;

  position(int row = 0, int col = 0) : row(row), col(col) {}

  position operator+(const position &other) const
  {
    return position(row + other.row, col + other.col);
  }

  position operator*(int scalar) const
  {
    return position(row * scalar, col * scalar);
  }

  position operator-(const position &other) const
  {
    return position(row - other.row, col - other.col);
  }

  position &operator+=(const position &other)
  {
    row += other.row;
    col += other.col;
    return *this;
  }

  bool operator==(const position &other) const
  {
    return row == other.row && col == other.col;
  }

  bool operator!=(const position &other) const
  {
    return !(*this == other);
  }
};
