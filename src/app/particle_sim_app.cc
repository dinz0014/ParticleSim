#include <numeric>

#include "common/utils.h"
#include "physics/particle_manager.h"
#include "render/renderer.h"

#include "particle_sim_app.h"

void ParticleSimApp::Run()
{
    sf::RenderWindow window{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulation", sf::Style::Titlebar | sf::Style::Close};
    sim::Container container{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    // Center the container
    container.centerInside({0.0f, static_cast<float>(WINDOW_WIDTH)}, {0.0f, static_cast<float>(WINDOW_HEIGHT)});

    sim::Renderer renderer{window};
    sim::ParticleManager manager{container};

    window.setFramerateLimit(TARGET_FPS);
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

        for (uint8_t i = SUBSTEPS; i > 0; i--)
        {
            manager.updateParticles(dt);
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