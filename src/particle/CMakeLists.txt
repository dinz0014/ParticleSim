
file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_library(particle STATIC ${SOURCES} ${HEADERS})

target_include_directories(particle PRIVATE ${CMAKE_SOURCE_DIR}/src)

find_package(SFML 2.6.1 COMPONENTS system window graphics REQUIRED)

target_link_libraries(particle PRIVATE sfml-system sfml-window sfml-graphics SimVector)