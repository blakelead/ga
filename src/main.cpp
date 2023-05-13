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
  Vector2 anchor_params = {600, 292};
  bool run_ga = false;

  params::instance().load("parameters.toml");
  auto p = params::instance().get();

  InitWindow(p.window.width, p.window.height, "ga");
  SetTargetFPS(30);

  maze_gen mg;
  auto maze = mg.generate(p.maze.rows, p.maze.cols, (maze_type)p.maze.type);
  ga ga(p.genetic.population_size, p.genetic.genome_size, p.genetic.crossover_rate, p.genetic.mutation_rate);
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

        GuiGroupBox((Rectangle){anchor_actions.x, anchor_actions.y, 192, 276}, "ACTIONS");
        if (GuiButton((Rectangle){anchor_actions.x + 12, anchor_actions.y + 24, 168, 24}, "GENERATE MAZE"))
        {
          ga.reset();
          path.directions.clear();
          maze = mg.generate(p.maze.rows, p.maze.cols, (maze_type)p.maze.type);
        }
        if (GuiButton((Rectangle){anchor_actions.x + 12, anchor_actions.y + 72, 168, 24}, "RUN GA"))
        {
          run_ga = true;
        }
        if (GuiButton((Rectangle){anchor_actions.x + 12, anchor_actions.y + 120, 168, 24}, "STOP GA"))
        {
          run_ga = false;
        }
        if (GuiButton((Rectangle){anchor_actions.x + 12, anchor_actions.y + 168, 168, 24}, "RESTART_GA"))
        {
          ga.reset();
          path.directions.clear();
        }

        GuiGroupBox((Rectangle){anchor_params.x, anchor_params.y, 192, 300}, "DATA");
        GuiLabel((Rectangle){anchor_params.x + 8, anchor_params.y + 8, 120, 24}, "GENERATION");
        const char *generation = TextFormat("%d", ga.get_curent_generation());
        if (GuiTextBox((Rectangle){anchor_params.x + 84, anchor_params.y + 8, 100, 24}, const_cast<char *>(generation), 128, false))
        {
        }
        GuiLabel((Rectangle){anchor_params.x + 8, anchor_params.y + 40, 120, 24}, "FITNESS");
        const char *fitness = TextFormat("%f", ga.get_current_generation_best());
        if (GuiTextBox((Rectangle){anchor_params.x + 84, anchor_params.y + 40, 100, 24}, const_cast<char *>(fitness), 128, false))
        {
        }
        GuiLabel((Rectangle){anchor_params.x + 8, anchor_params.y + 72, 120, 24}, "GENOME SIZE");
        const char *genome_size = TextFormat("%d", path.directions.size());
        if (GuiTextBox((Rectangle){anchor_params.x + 84, anchor_params.y + 72, 100, 24}, const_cast<char *>(genome_size), 128, false))
        {
        }
      }

      maze.draw(584, 584, 8, 8);
      maze.draw_path(584, 584, 8, 8, path);
      // DrawText(TextFormat("generation: %d", ga.get_curent_generation()), 15, 15, 20, LIME);
      // DrawText(TextFormat("fitness: %f", ga.get_current_generation_best()), 15, 35, 20, LIME);
      // DrawText(TextFormat("size: %d", path.directions.size()), 15, 55, 20, LIME);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
