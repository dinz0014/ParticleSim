#include "utils/utils.h"
#include "particle_sim_app.h"

ParticleSimApp::ParticleSimApp()
    : manager_{timestep_, WINDOW_HEIGHT, WINDOW_WIDTH}
{
}

void ParticleSimApp::drawGrid(sf::RenderWindow& window)
{
    sf::Color lineColor(200, 200, 200); // Light gray color for grid lines
    const int GRID_SIZE_X = 16;
    const int GRID_SIZE_Y = 16;
    const int CELL_SIZE_X = WINDOW_WIDTH / 16;
    const int CELL_SIZE_Y = WINDOW_HEIGHT / 16;

    // Draw vertical lines
    for (int x = 0; x <= GRID_SIZE_X; ++x) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x * CELL_SIZE_X, 0)),
            sf::Vertex(sf::Vector2f(x * CELL_SIZE_X, WINDOW_HEIGHT))
        };
        line[0].color = lineColor;
        line[1].color = lineColor;
        window.draw(line, 2, sf::Lines);
    }

    // Draw horizontal lines
    for (int y = 0; y <= GRID_SIZE_Y; ++y) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, y * CELL_SIZE_Y)),
            sf::Vertex(sf::Vector2f(WINDOW_WIDTH, y * CELL_SIZE_Y))
        };
        line[0].color = lineColor;
        line[1].color = lineColor;
        window.draw(line, 2, sf::Lines);
    }

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

        while (accumulator_ >= timestep_)
        {
            manager_.updateParticles();
            accumulator_ -= timestep_;
        }

        window.clear();

        for (const auto& particle : deref_non_null_view(manager_.particles()))
        {
            window.draw(particle.shape());
        }

        // drawGrid(window);

        window.display();
    }
}