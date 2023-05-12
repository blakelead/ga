#pragma once

// singleton that loads parameters file and make parameters
// accessible anywhere in the code

#include <spdlog/spdlog.h>
#include <toml++/toml.h>

struct params_data
{
  struct
  {
    int width;
    int height;
  } window;
  struct
  {
    int cols;
    int rows;
    int type;
  } maze;
  struct
  {
    int population_size;
    int genome_size;
    float crossover_rate;
    float mutation_rate;
  } genetic;
};

class params
{
public:
  static params &instance()
  {
    static params *instance = new params();
    return *instance;
  }

  void load(const char *file)
  {
    auto p = toml::parse_file(file);
    m_data.window.width = p["window"]["width"].value_or(0);
    m_data.window.height = p["window"]["height"].value_or(0);

    m_data.maze.rows = p["maze"]["rows"].value_or(0);
    m_data.maze.cols = p["maze"]["cols"].value_or(0);
    m_data.maze.type = p["maze"]["type"].value_or(0);

    m_data.genetic.population_size = p["genetic"]["population_size"].value_or(0);
    m_data.genetic.genome_size = p["genetic"]["genome_size"].value_or(0);
    m_data.genetic.crossover_rate = p["genetic"]["crossover_rate"].value_or(0.0f);
    m_data.genetic.mutation_rate = p["genetic"]["mutation_rate"].value_or(0.0f);
  }

  const params_data &get()
  {
    return m_data;
  }

private:
  params() {}
  params_data m_data;
};
