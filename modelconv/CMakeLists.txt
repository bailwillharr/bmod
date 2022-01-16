cmake_minimum_required(VERSION 3.10)
project(modelconv)

add_executable(modelconv src/main.c)

target_link_libraries(modelconv PRIVATE assimp)