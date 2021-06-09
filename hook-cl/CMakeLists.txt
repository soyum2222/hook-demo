cmake_minimum_required(VERSION 3.17)
project(hook_cl C)

set(CMAKE_C_STANDARD 99)

add_executable(hook_cl main.c detours.lib)