#include "particle_sim_app.h"

ParticleSimApp::ParticleSimApp() : manager_{target_fps_}
{
}

void ParticleSimApp::Run()
{
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr float fps{144.0};
    constexpr float timestep = 1.0 / fps;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulation");
    std::vector<Particle> particles;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                const auto &particle = manager_.CreateParticleAtCursor(event.mouseButton);
                window.draw(particle.shape());
            }
        }

        window.clear();

        for (const auto &particle : manager_.particles())
        {
            window.draw(particle->shape());
        }

        window.display();
    }
}