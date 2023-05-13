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

  genome(const genome &other) : data(other.data), fitness(other.fitness) {}

  genome &operator=(const genome &other)
  {
    if (this != &other)
    {
      this->data = other.data;
      this->fitness = other.fitness;
    }
    return *this;
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
