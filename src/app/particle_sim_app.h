#pragma once
#include <SFML/Graphics.hpp>

class ParticleSimApp
{
public:
    void Run();

private:
    static constexpr int TARGET_FPS = 60;
    static constexpr int WINDOW_WIDTH = 1920;
    static constexpr int WINDOW_HEIGHT = 1080;

    static constexpr float TIMESTEP = 1.0f / TARGET_FPS;
    static constexpr uint8_t SUBSTEPS = 16;
    static constexpr float dt = TIMESTEP / static_cast<float>(SUBSTEPS);
};