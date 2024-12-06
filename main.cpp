#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include <random>
int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Test");
    std::array<sf::Color, 3> colors = {sf::Color::Blue,sf::Color::Red,sf::Color::Green};
    // RectangleShape setup
    sf::RectangleShape line(sf::Vector2f(150.f, 5.f));
    sf::RectangleShape line2(sf::Vector2f(100.f,100.f));

    line.setOrigin(line.getSize().x / 2, line.getSize().y / 2);
    line.setPosition(100.f, 100.f); // Center of the window
    line.setFillColor(sf::Color::Blue); // Distinguishable color
    line.rotate(45.f);

    line2.setOrigin(line2.getSize().x / 2, line2.getSize().y / 2);
    line2.setPosition(100.f, 100.f); // Center of the window
    line2.setFillColor(sf::Color::Red); // Distinguishable color
    line2.rotate(45.f);

    // gets 'entropy' from device that generates random numbers itself
    // to seed a mersenne twister (pseudo) random generator
    std::mt19937 generator(std::random_device{}());

    // make sure all numbers have an equal chance. 
    // range is inclusive (so we need -1 for vector index)
    std::uniform_int_distribution<std::size_t> distribution(0, colors.size() - 1);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
            window.close();
            std::cout << "Handling event closed" << std::endl;
            exit(EXIT_SUCCESS);
            }

            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.type == sf::Event::MouseLeft) {
                    std::cout << "Left Mouse Button Pressed" << std::endl;
                }
            }

            if(event.type == sf::Event::KeyReleased) {
                
                if(event.key.code == sf::Keyboard::R) {
                    std::cout << "Key Pressed" << std::endl;
                    std::size_t number = distribution(generator);
                    line.setFillColor(colors[number]);
                    line2.setFillColor(colors[number]);
                }
            }
        }

        window.clear(sf::Color::Black); // Clear with black background
        window.draw(line); // Draw only the rectangle
        window.draw(line2); //Drawing the point
        window.display();
    }

    return 0;
}
