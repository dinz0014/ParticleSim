
file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_library(physics STATIC ${SOURCES} ${HEADERS})

target_include_directories(physics PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(physics PRIVATE sfml-system sfml-window sfml-graphics)