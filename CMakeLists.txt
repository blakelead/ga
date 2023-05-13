cmake_minimum_required(VERSION 3.25)

project(ga)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
# raylib
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 4.5.0)
set(raylib_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)
# raygui
FetchContent_Declare(raygui GIT_REPOSITORY https://github.com/raysan5/raygui.git GIT_TAG 3.6 SOURCE_SUBDIR projects/CMake)
FetchContent_MakeAvailable(raygui)
# log
FetchContent_Declare(log GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.11.0)
FetchContent_MakeAvailable(log)
# toml
FetchContent_Declare(toml GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git GIT_TAG v3.3.0)
FetchContent_MakeAvailable(toml)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/maze/maze_gen.cpp
  src/maze/maze.cpp
  src/genetic/ga.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  src
  ${raygui_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
  raylib
  raygui
  spdlog
  tomlplusplus::tomlplusplus
)
