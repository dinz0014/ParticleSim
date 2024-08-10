#include "common/utils.h"
#include <iostream>
#include "renderer.h"

namespace sim {

Renderer::Renderer(const json& cfg, sf::RenderWindow& window)
        : window_{window}
{
    cfg_ = cfg;
    LoadConfig();
}

ValidationResult Renderer::ValidateConfig(const json& renderer_cfg)
{
    if (renderer_cfg.contains("particle_colour"))
    {
        const json& col = renderer_cfg["particle_colour"];

        if (col.size() != 4 || !(col.contains("r") && col.contains("g") && col.contains("b") && col.contains("a")))
        {
            return "particle_colour should contain 4 values: one each for r,g,b,a components";
        }

        for (auto component : col)
        {
            if (!component.is_number_unsigned())
            {
                return "the r,g,b,a values for the colour must be unsigned integers";
            }

            component = static_cast<uint8_t>(component);
            if (component > 255)
            {
                return "the r,g,b,a values for the colour must be in the range [0, 255]";
            }
        }
    }

    if (renderer_cfg.contains("container_wall_thickness")
        && !renderer_cfg["container_wall_thickness"].is_number_float()
        && renderer_cfg["container_wall_thickness"] < 0.0f)
    {
        return "container_wall_thickness must be a positive floating point number";
    }

    return "";
}

void Renderer::LoadConfig()
{
    if (cfg_.contains("particle_colour"))
    {
        uint8_t r, g, b, a;
        const json& colour_cfg = cfg_["particle_colour"];

        r = colour_cfg["r"];
        g = colour_cfg["g"];
        b = colour_cfg["b"];
        a = colour_cfg["a"];

        particle_colour_ = sf::Color(r, g, b, a);
    }

    container_thickness_ = get(cfg_, "container_wall_thickness", 2.0f);
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