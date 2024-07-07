#include "renderer.h"

namespace sim {

const sf::Color Renderer::slowColour = sf::Color::Cyan;
const sf::Color Renderer::fastColour = sf::Color::Magenta;
const sf::Color Renderer::midColour = sf::Color(173, 255, 47); // Pastel Green

Renderer::Renderer(sf::RenderWindow& window)
        : window_{window}
{
}

void Renderer::drawParticle(const Particle& particle)
{
    sf::CircleShape shape{particle.radius()};
    shape.setOrigin({particle.radius(), particle.radius()});
    shape.setPosition(particle.position());
    shape.setFillColor(computeFillColour(particle));
    window_.draw(shape);
}

void Renderer::drawContainer(const Container& container)
{
    const Vec2f c_size = Vec2f::from(container.getSize());
    sf::RectangleShape c_shape{c_size};
    Vec2f c_center = c_size / 2.0f;

    c_shape.setOrigin(c_center);
    c_shape.setPosition(container.position());
    c_shape.setFillColor(sf::Color::Transparent);
    c_shape.setOutlineThickness(2.0f);

    window_.draw(c_shape);
}

sf::Color Renderer::computeFillColour(const Particle& particle)
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

}