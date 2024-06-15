cmake_minimum_required(VERSION 3.10)

project(ParticleSim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/particle)
add_subdirectory(src/app)
add_subdirectory(src/vector)