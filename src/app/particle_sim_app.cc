#include <fstream>
#include <iostream>
#include <numeric>

#include "common/utils.h"
#include "physics/particle_manager.h"
#include "render/renderer.h"

#include "particle_sim_app.h"

sim::ValidationResult ParticleSimApp::ValidateConfig()
{
    if (!cfg_.contains("app"))
    {
        return "app config is required, but absent";
    }

    if (!cfg_.contains("particle_manager"))
    {
        return "particle_manager config is required, but absent";
    }

    const json& app_cfg = cfg_["app"];

    if (!app_cfg.contains("solver_substeps"))
    {
        return "solver_substeps field is required, but absent";
    }

    if (!app_cfg["solver_substeps"].is_number_integer() && !(app_cfg["solver_substeps"] > 0))
    {
        return "solver_substeps must be a positive integer";
    }

    if (app_cfg.contains("target_fps") && !app_cfg["target_fps"].is_number_integer() && !(app_cfg["target_fps"] > 0))
    {
        return "target_fps must be a positive integer";
    }

    return "";
}

void ParticleSimApp::ReadConfig(const std::string& filename)
{
    std::ifstream config_ifs(filename);
    cfg_ = json::parse(config_ifs);
    if (const auto err = ValidateConfig(); !err.empty())
    {
        throw std::runtime_error(err);
    }

    LoadConfig();
}

void ParticleSimApp::LoadConfig()
{
    const json& app_cfg = cfg_["app"];
    target_fps_ = get(app_cfg, "target_fps", 60);
    substeps_ = get(app_cfg, "solver_substeps", 16);
    dt_ = (1.0f / static_cast<float>(target_fps_)) / static_cast<float>(substeps_);

    // Insert empty renderer config if there is no renderer config for convenience
    if (!cfg_.contains("renderer"))
    {
        cfg_["renderer"] = {};
    }
}

void ParticleSimApp::Run()
{
    sf::RenderWindow window{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulation", sf::Style::Titlebar | sf::Style::Close};
    sim::Container container{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    // Center the container
    container.centerInside({0.0f, static_cast<float>(WINDOW_WIDTH)}, {0.0f, static_cast<float>(WINDOW_HEIGHT)});

    if (const auto err = sim::Renderer::ValidateConfig(cfg_["renderer"]); !err.empty())
    {
        throw std::runtime_error(err);
    }

    const json& render_cfg = cfg_["renderer"];
    sim::Renderer renderer{render_cfg, window};

    if (const auto err = sim::ParticleManager::ValidateConfig(cfg_["particle_manager"]); !err.empty())
    {
        throw std::runtime_error(err);
    }

    const json& manager_cfg = cfg_["particle_manager"];
    sim::ParticleManager manager{manager_cfg, container};

    window.setFramerateLimit(target_fps_);
    bool left_mouse_held = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Detect dragging
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                left_mouse_held = true;
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                left_mouse_held = false;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                manager.clear();
            }
        }

        if (left_mouse_held)
        {
            auto [x, y] = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (container.intersects(x, y))
            {
                const auto& particle = manager.createParticleAtCursor(x, y);
                renderer.drawParticle(particle);
            }
        }

        for (uint8_t i = substeps_; i > 0; i--)
        {
            manager.updateParticles(dt_);
        }

        window.clear();

        renderer.drawContainer(container);

        for (const auto& particle : manager.particles())
        {
            renderer.drawParticle(particle);
        }

        // Set title with particle count and the average speed of the particles
        const auto count = manager.particle_count();
        std::string title = "Particles: " + std::to_string(count);
        title += " | Avg Speed: ";

        auto cumulative_speed = [](double s, const sim::Particle& p) -> double { return s + p.velocity().magnitude(); };
        const double avg_speed = std::accumulate(manager.particles().begin(), manager.particles().end(), 0.0, cumulative_speed) / static_cast<double>(count);
        title += std::to_string(std::round(avg_speed * 1000.0) / 1000.0);

        window.setTitle(title);

        window.display();
    }
}