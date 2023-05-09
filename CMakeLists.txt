cmake_minimum_required(VERSION 3.25)

project(ga VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
# raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 4.5.0)
FetchContent_MakeAvailable(raylib)
# log
FetchContent_Declare(log GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.11.0)
FetchContent_MakeAvailable(log)
# toml
FetchContent_Declare(toml GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git GIT_TAG v3.3.0)
FetchContent_MakeAvailable(toml)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PUBLIC
  raylib
  spdlog
  tomlplusplus::tomlplusplus
)
