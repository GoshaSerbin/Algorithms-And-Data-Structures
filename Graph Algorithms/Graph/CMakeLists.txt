cmake_minimum_required(VERSION 3.0.0)
set(ProjectName graph)
project(${ProjectName} VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SOURCES main.cpp)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_options(-fsanitize=address)

    add_link_options(-fsanitize=address)
endif()

add_executable(${ProjectName} ${SOURCES})