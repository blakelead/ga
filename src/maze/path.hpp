#pragma once

#include "maze/position.hpp"
#include <vector>

struct path
{
  std::vector<position> directions;
  int size() const
  {
    return directions.size();
  }
};
