#include "genetic/ga.hpp"
#include "maze/maze_gen.hpp"
#include "maze/path.hpp"
#include "params.hpp"
#include <raylib.h>

int main()
{
  srand(time(nullptr));
  spdlog::set_level(spdlog::level::debug);

  params::instance().load("parameters.toml");
  auto p = params::instance().get();

  InitWindow(p.window.width, p.window.height, "ga");
  SetTargetFPS(30);

  maze_gen mg;
  auto maze = mg.generate(p.maze.rows, p.maze.cols, (maze_type)p.maze.type);

  ga ga(p.genetic.population_size, p.genetic.genome_size, p.genetic.crossover_rate, p.genetic.mutation_rate);

  path path = ga.generation(maze);

  while (!WindowShouldClose())
  {
    if (ga.get_current_generation_best() < 1)
    {
      path = ga.generation(maze);
    }
    BeginDrawing();
    {
      ClearBackground(DARKGRAY);

      maze.draw(p.window.width, p.window.height);
      maze.draw_path(p.window.width, p.window.height, path);
      DrawText(TextFormat("generation: %d", ga.get_curent_generation()), 15, 15, 20, LIME);
      DrawText(TextFormat("fitness: %f", ga.get_current_generation_best()), 15, 35, 20, LIME);
      DrawText(TextFormat("size: %d", path.directions.size()), 15, 55, 20, LIME);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
