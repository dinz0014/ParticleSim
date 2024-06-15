#include "particle_sim_app.h"

ParticleSimApp::ParticleSimApp() : manager_{timestep_}
{
}

void ParticleSimApp::Run()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulation");

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

        // float time_elapsed = clock_.restart().asSeconds();
        // accumulator_ += time_elapsed;

        // while (accumulator_ >= timestep_)
        // {
        //     manager_.updateParticles();
        // }

        window.clear();

        for (const auto &particle : manager_.particles())
        {
            window.draw(particle->shape());
        }

        window.display();
    }
}