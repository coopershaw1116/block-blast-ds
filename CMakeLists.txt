cmake_minimum_required(VERSION 3.16)

project(BLOCK-BLAST-DS C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable(BLOCK-BLAST-DS
    src/core/main.c
    src/core/game.c
    src/core/piece.c
    src/rendering/render.c
    src/input/input.c
)

target_include_directories(BLOCK-BLAST-DS PRIVATE src/core src/rendering src/input)

target_link_libraries(BLOCK-BLAST-DS PRIVATE SDL2::SDL2)