#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");
    std::vector<sf::CircleShape> particles;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                sf::CircleShape circle(5.0f);
                circle.setPosition(x, y);
                circle.setFillColor(sf::Color::Cyan);
                particles.push_back(std::move(circle));
            }
        }

        window.clear();

        for (auto &particle : particles)
        {
            window.draw(particle);
        }

        window.display();
    }

    return 0;
}