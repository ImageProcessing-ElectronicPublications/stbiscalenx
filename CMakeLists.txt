cmake_minimum_required(VERSION 2.8)
project(stbiscalenx)
add_compile_options(-O3)
include_directories(src)
add_executable(stbiscalenx src/stbiscalenx.c src/dependencies.c)
target_link_libraries(stbiscalenx m)
