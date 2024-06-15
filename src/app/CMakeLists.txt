add_executable(ParticleSimulation main.cc particle_sim_app.cc)

target_include_directories(ParticleSimulation PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ParticleSimulation PUBLIC particle sfml-system sfml-window sfml-graphics)