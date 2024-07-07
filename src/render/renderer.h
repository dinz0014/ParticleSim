#pragma once
#include <SFML/Graphics.hpp>
#include "physics/particle.h"

namespace sim {

class Renderer
{
public:
    static const sf::Color slowColour;
    static const sf::Color fastColour;
    static const sf::Color midColour;

    Renderer(sf::RenderWindow& window);

    void drawParticle(const Particle& particle);
    void drawContainer(const Container& container);

private:
    sf::Color computeFillColour(const Particle& particle);

    sf::RenderWindow& window_;
};

}