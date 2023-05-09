#include "params.hpp"
#include <ctime>
#include <raylib.h>
#include <spdlog/spdlog.h>

int main()
{
  SetRandomSeed(time(nullptr));

  params::instance().load("parameters.toml");
  auto p = params::instance().get();

  InitWindow(p.window.width, p.window.height, "ga");
  SetTargetFPS(30);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    {
      ClearBackground(DARKGRAY);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
