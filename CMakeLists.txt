cmake_minimum_required(VERSION 3.10)

project(ParticleSim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 2.6.1 COMPONENTS system window graphics REQUIRED)

add_subdirectory(src/physics)
add_subdirectory(src/app)
add_subdirectory(src/render)