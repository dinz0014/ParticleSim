add_library(particle STATIC particle.cc)

target_include_directories(particle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

target_link_libraries(particle PUBLIC sfml-system sfml-window sfml-graphics)