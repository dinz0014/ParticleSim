#include "common/utils.h"
#include "physics/particle_manager.h"
#include "render/renderer.h"

#include "particle_sim_app.h"

void ParticleSimApp::Run()
{
    sf::RenderWindow window{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulation", sf::Style::Titlebar | sf::Style::Close};
    sim::Container container{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    sim::Renderer renderer{window};
    sim::ParticleManager manager{container};

    // Center the container
    container.centerInside({0.0f, static_cast<float>(WINDOW_WIDTH)}, {0.0f, static_cast<float>(WINDOW_HEIGHT)});

    window.setFramerateLimit(TARGET_FPS);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                float x = event.mouseButton.x;
                float y = event.mouseButton.y;

                if (container.intersects(x, y))
                {
                    const auto& particle = manager.createParticleAtCursor(x, y);
                    renderer.drawParticle(particle);
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.control)
                {
                    container.handleResize(event.key);
                }
            }
        }

        for (uint8_t i = SUBSTEPS; i > 0; i--)
        {
            manager.updateParticles(dt);
        }

        window.clear();

        renderer.drawContainer(container);

        for (const auto& particle : deref_non_null_view(manager.particles()))
        {
            renderer.drawParticle(particle);
        }

        window.display();
    }
}