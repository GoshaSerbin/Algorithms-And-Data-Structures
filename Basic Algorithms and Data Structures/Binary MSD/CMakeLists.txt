cmake_minimum_required(VERSION 3.5.0)
project(task)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Werror -Wpedantic)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
