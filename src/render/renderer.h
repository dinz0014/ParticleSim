#pragma once
#include <SFML/Graphics.hpp>
#include "physics/particle.h"

namespace sim {

class Renderer
{
public:
    Renderer(sf::RenderWindow& window);

    void drawParticle(const Particle& particle);
    void drawContainer(const Container& container);

private:
    sf::RenderWindow& window_;
};

}