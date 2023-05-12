#include "ga.hpp"
#include "maze/maze.hpp"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <functional>

ga::ga(int population_size, int genome_size, float crossover_rate, float mutation_rate)
    : m_population_size(population_size), m_genome_size(genome_size), m_gene_size(2),
      m_crossover_rate(crossover_rate), m_mutation_rate(mutation_rate)
{
  for (int i = 0; i < population_size; i++)
  {
    m_genomes.push_back(genome(genome_size * m_gene_size));
  }
}

path ga::generation(const maze &maze)
{
  // compute fitness for all genomes
  for (auto &genome : m_genomes)
  {
    genome.fitness = maze.test_path(decode(genome));
  }

  // sort genomes by fitness
  std::sort(m_genomes.begin(), m_genomes.end(), std::greater<genome>());

  // initialize the children genomes vector
  std::vector<genome> children;
  children.reserve(m_genomes.size());

  // add the elite genomes to the children
  children.push_back(m_genomes[0]);
  children.push_back(m_genomes[1]);

  while (children.size() < m_genomes.size())
  {
    genome child = crossover(select(), select());
    mutate(child);
    children.push_back(child);
  }
  m_generation++;
  m_genomes = std::move(children);
  return decode(m_genomes[0]);
}

float ga::get_current_generation_best()
{
  return m_genomes[0].fitness;
}

int ga::get_curent_generation()
{
  return m_generation;
}

const genome &ga::select()
{
  // only select the best first quarter of the genomes
  const genome &genome1 = m_genomes[rand() % (m_genomes.size() / 2)];
  const genome &genome2 = m_genomes[rand() % (m_genomes.size() / 2)];

  return genome1 > genome2 ? genome1 : genome2;
}

genome ga::crossover(const genome &parent1, const genome &parent2)
{
  // one point crossover for now
  int crossover_point = rand() % std::min(parent1.size(), parent2.size());
  genome child;
  child.data.insert(child.data.end(), parent1.data.begin(), parent1.data.begin() + crossover_point);
  child.data.insert(child.data.end(), parent2.data.begin() + crossover_point, parent2.data.end());
  return child;
}

void ga::mutate(genome &genome)
{
  // flip mutation
  for (int i = 0; i < genome.size(); i++)
  {
    float r = (float)(rand()) / (float)(RAND_MAX);
    if (r < m_mutation_rate)
    {
      genome.data[i] = (rand() % 2);
    }
  }
  // // deletion mutation
  // float dr = (float)(rand()) / (float)(RAND_MAX);
  // if (dr < m_mutation_rate)
  // {
  //   int index = rand() % genome.size() - 1;
  //   genome.data.erase(genome.data.begin() + index, genome.data.begin() + index + 1);
  // }
  // insertion mutation
  float ir = (float)(rand()) / (float)(RAND_MAX);
  if (ir < m_mutation_rate)
  {
    int index = rand() % genome.size();
    int bit1 = rand() % 2;
    int bit2 = rand() % 2;
    genome.data.insert(genome.data.begin() + index, bit1);
    genome.data.insert(genome.data.begin() + index, bit2);
  }
}

path ga::decode(const genome &genome) const
{
  path path;
  for (int i = 0; i < genome.size(); i += m_gene_size)
  {
    std::vector<position> directions = {position(-1, 0), position(0, 1), position(1, 0), position(0, -1)};
    int dir = genome[i] * 2 + genome[i + 1];
    path.directions.push_back({directions[dir].row, directions[dir].col});
  }
  return path;
}
