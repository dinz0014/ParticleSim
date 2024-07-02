#pragma once
#include "particle/particle.h"
#include <SFML/Graphics.hpp>

class ParticleSimApp
{
public:
    ParticleSimApp();
    void Run();

private:
    void drawGrid(sf::RenderWindow& window);

    static constexpr int TARGET_FPS = 100;
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;

    static constexpr float timestep_ = 1.0f / TARGET_FPS;
    float accumulator_{0.0f};
    sf::Clock clock_;
    sim::ParticleManager manager_;
};