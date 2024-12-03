#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Test");

    // RectangleShape setup
    sf::RectangleShape line(sf::Vector2f(150.f, 5.f));
    line.setOrigin(line.getSize().x / 2, line.getSize().y / 2);
    line.setPosition(100.f, 100.f); // Center of the window
    line.setFillColor(sf::Color::Blue); // Distinguishable color
    line.rotate(45.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
            window.close();
            std::cout << "Handling event closed" <<std::endl;
            exit(EXIT_SUCCESS);
            }
        }

        window.clear(sf::Color::Black); // Clear with black background
        window.draw(line); // Draw only the rectangle
        window.display();
    }

    return 0;
}
