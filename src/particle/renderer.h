#pragma once
#include <SFML/Graphics.hpp>
#include "particle.h"

namespace sim {

class ParticleRenderer
{
public:
    static const sf::Color slowColour;
    static const sf::Color fastColour;
    static const sf::Color midColour;

    ParticleRenderer(sf::RenderWindow& window)
        : window_{window}
    {
    }

    void drawParticle(const Particle& particle)
    {
        sf::CircleShape shape(particle.radius());
        shape.setOrigin({particle.radius(), particle.radius()});
        shape.setPosition(particle.position());
        shape.setFillColor(computeFillColour(particle));
        window_.draw(shape);
    }

private:
    sf::Color computeFillColour(const Particle& particle)
    {
        const auto normalised_speed =
            std::clamp(vec_dot(particle.velocity(), particle.velocity()) / (MAX_VEL * MAX_VEL), 0.0f, 1.0f);

        auto interpolate_colour = [](float start, float end, float val)
        {
            return static_cast<sf::Uint8>(start + val * (end - start));
        };

        auto final_colour = [&](sf::Color start_colour, sf::Color end_colour)
        {
            return sf::Color(
                interpolate_colour(start_colour.r, end_colour.r, normalised_speed),
                interpolate_colour(start_colour.g, end_colour.g, normalised_speed),
                interpolate_colour(start_colour.b, end_colour.b, normalised_speed)
            );
        };

        // Adjust the 0.7 here for colour gradient
        if (normalised_speed < 0.7f)
        {
            return final_colour(slowColour, midColour);
        }
        else
        {
            return final_colour(midColour, fastColour);
        }
    }

    sf::RenderWindow& window_;

};

const sf::Color ParticleRenderer::slowColour = sf::Color::Cyan;
const sf::Color ParticleRenderer::fastColour = sf::Color::Magenta;
const sf::Color ParticleRenderer::midColour = sf::Color(173, 255, 47); // Pastel Green

}