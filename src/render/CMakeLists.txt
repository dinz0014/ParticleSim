file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_library(render STATIC ${SOURCES} ${HEADERS})

target_include_directories(
    render
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/include
)

target_link_libraries(render PRIVATE sfml-system sfml-window sfml-graphics)