#include "genetic/ga.hpp"
#include "maze/maze_gen.hpp"
#include "maze/path.hpp"
#include "params.hpp"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main()
{
  srand(time(nullptr));
  spdlog::set_level(spdlog::level::debug);

  // GUI
  Vector2 anchor_maze = {8, 8};
  Vector2 anchor_actions = {600, 8};
  Vector2 anchor_params = {600, 120};
  Vector2 anchor_data = {600, 264};
  bool run_ga = false;

  params::instance().load("parameters.toml");
  auto p = params::instance().get();

  float crossover_rate = p.genetic.crossover_rate;
  float mutation_rate = p.genetic.mutation_rate;
  int population_size = p.genetic.population_size;
  int genome_size = p.genetic.genome_size;

  InitWindow(p.window.width, p.window.height, "ga");
  SetTargetFPS(30);

  maze_gen mg;
  auto maze = mg.generate(p.maze.rows, p.maze.cols, (maze_type)p.maze.type);
  ga ga(population_size, genome_size, crossover_rate, mutation_rate);
  path path;

  while (!WindowShouldClose())
  {
    if (run_ga && ga.get_current_generation_best() < 1)
    {
      path = ga.generation(maze);
    }
    BeginDrawing();
    {
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      // GUI
      {
        GuiGroupBox((Rectangle){anchor_maze.x, anchor_maze.y, 584, 584}, "MAZE");

        GuiGroupBox((Rectangle){anchor_actions.x, anchor_actions.y, 192, 104}, "ACTIONS");
        if (GuiButton((Rectangle){anchor_actions.x + 12, anchor_actions.y + 8, 168, 24}, "RUN"))
        {
          run_ga = true;
          ga.reset(population_size, genome_size, crossover_rate, mutation_rate);
          path.directions.clear();
        }
        if (GuiButton((Rectangle){anchor_actions.x + 12, anchor_actions.y + 40, 168, 24}, "STOP"))
        {
          run_ga = false;
        }
        if (GuiButton((Rectangle){anchor_actions.x + 12, anchor_actions.y + 72, 168, 24}, "NEW MAZE"))
        {
          run_ga = false;
          ga.reset(population_size, genome_size, crossover_rate, mutation_rate);
          path.directions.clear();
          maze = mg.generate(p.maze.rows, p.maze.cols, (maze_type)p.maze.type);
        }

        GuiGroupBox((Rectangle){anchor_params.x, anchor_params.y, 192, 136}, "PARAMETERS");
        const char *crossover = TextFormat("%.1f", crossover_rate);
        const char *mutation = TextFormat("%.3f", mutation_rate);
        const char *population = TextFormat("%d", population_size);
        const char *genome = TextFormat("%d", genome_size);
        crossover_rate = GuiSliderPro((Rectangle){anchor_params.x + 34, anchor_params.y + 8, 124, 24}, "C.O.", crossover, crossover_rate, 0, 1, 24);
        mutation_rate = GuiSliderPro((Rectangle){anchor_params.x + 34, anchor_params.y + 40, 124, 24}, "MUT.", mutation, mutation_rate, 0, 1, 24);
        population_size = GuiSliderPro((Rectangle){anchor_params.x + 34, anchor_params.y + 72, 124, 24}, "POP.", population, population_size, 10, 1000, 24);
        genome_size = GuiSliderPro((Rectangle){anchor_params.x + 34, anchor_params.y + 104, 124, 24}, "GEN.", genome, genome_size, 1, 500, 24);

        GuiGroupBox((Rectangle){anchor_data.x, anchor_data.y, 192, 104}, "DATA");
        const char *generation = TextFormat("%d", ga.get_curent_generation());
        GuiLabel((Rectangle){anchor_data.x + 8, anchor_data.y + 8, 120, 24}, "GENERATION");
        GuiTextBox((Rectangle){anchor_data.x + 84, anchor_data.y + 8, 100, 24}, const_cast<char *>(generation), 128, false);
        const char *fitness = TextFormat("%f", ga.get_current_generation_best());
        GuiLabel((Rectangle){anchor_data.x + 8, anchor_data.y + 40, 120, 24}, "FITNESS");
        GuiTextBox((Rectangle){anchor_data.x + 84, anchor_data.y + 40, 100, 24}, const_cast<char *>(fitness), 128, false);
        const char *genome_size = TextFormat("%d", path.directions.size());
        GuiLabel((Rectangle){anchor_data.x + 8, anchor_data.y + 72, 120, 24}, "GENOME SIZE");
        GuiTextBox((Rectangle){anchor_data.x + 84, anchor_data.y + 72, 100, 24}, const_cast<char *>(genome_size), 128, false);
      }

      maze.draw(568, 568, 16, 16);
      maze.draw_path(568, 568, 16, 16, path);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
