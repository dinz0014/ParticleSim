#pragma once
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "physics/particle.h"
#include "types.h"

namespace sim {

class Renderer
{
public:
    using json = nlohmann::json;

    Renderer() = delete;
    Renderer(const json& cfg, sf::RenderWindow& window);

    static ValidationResult ValidateConfig(const json& render_cfg);
    void LoadConfig();

    void drawParticle(const Particle& particle);
    void drawContainer(const Container& container);

private:
    json cfg_;
    sf::RenderWindow& window_;

    sf::Color particle_colour_;
    float container_thickness_;
};

}