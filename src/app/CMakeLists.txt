file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_executable(ParticleSimulation ${SOURCES} ${HEADERS})

target_include_directories(ParticleSimulation PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ParticleSimulation PRIVATE
                        sfml-system
                        sfml-window
                        sfml-graphics
                        physics
                        render)