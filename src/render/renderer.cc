#include "renderer.h"

namespace sim {

Renderer::Renderer(sf::RenderWindow& window)
        : window_{window}
{
}

void Renderer::drawParticle(const Particle& particle)
{
    sf::CircleShape shape{particle.radius()};
    shape.setOrigin({particle.radius(), particle.radius()});
    shape.setPosition(particle.position());
    shape.setFillColor(sf::Color::Cyan);
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

}