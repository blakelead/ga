#pragma once

#include "genetic/genome.hpp"
#include "maze/maze.hpp"

class ga
{
public:
  ga(int population_size, int genome_size, float crossover_rate, float mutation_rate);
  
  void reset();
  path generation(const maze &maze);
  float get_current_generation_best();
  int get_curent_generation();

private:
  const genome &select();
  genome crossover(const genome &parent1, const genome &parent2);
  void mutate(genome &genome);
  path decode(const genome &genome) const;

private:
  std::vector<genome> m_genomes;

  int m_population_size;
  int m_genome_size;
  int m_gene_size;
  float m_crossover_rate;
  float m_mutation_rate;
  int m_generation;
};
