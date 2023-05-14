#pragma once

#include <vector>

struct genome
{
  std::vector<int> data;
  float fitness;

  genome() : fitness(0) {}

  genome(int size) : fitness(0)
  {
    for (int i = 0; i < size; i++)
    {
      data.push_back(rand() % 2);
    }
  }

  bool operator<(const genome &other) const
  {
    return fitness < other.fitness;
  }

  bool operator>(const genome &other) const
  {
    return fitness > other.fitness;
  }

  bool operator==(const genome &other) const
  {
    return fitness == other.fitness;
  }

  int operator[](int index) const
  {
    return data[index];
  }

  int size() const
  {
    return data.size();
  }
};
