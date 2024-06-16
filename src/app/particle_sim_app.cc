#include "particle_sim_app.h"

ParticleSimApp::ParticleSimApp()
    : manager_{timestep_, WINDOW_HEIGHT, WINDOW_WIDTH}
{
}

void ParticleSimApp::Run()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulation", sf::Style::Titlebar | sf::Style::Close);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                const auto& particle = manager_.createParticleAtCursor(event.mouseButton);
                window.draw(particle.shape());
            }
        }

        float time_elapsed = clock_.restart().asSeconds();
        accumulator_ += time_elapsed;
        int iter = 0;

        while (accumulator_ >= timestep_ && iter <= 5)
        {
            manager_.updateParticles();
            accumulator_ -= timestep_;
        }

        window.clear();

        for (const auto& region : manager_.particles())
        {
            for (const auto& particle : region)
            {
                window.draw(particle->shape());
            }
        }

        window.display();
    }
}