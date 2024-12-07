#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include <random>
#include <vector>
int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Test");
    std::array<sf::Color, 3> colors = {sf::Color::Blue,sf::Color::Red,sf::Color::Green};
    sf::Vertex vertex(sf::Vector2f(10.f, 50.f), sf::Color::Red, sf::Vector2f(100.f, 100.f));
    std::vector<sf::Vertex> vertices;
    vertices.push_back(vertex);

    //Text setup
    sf::Font font;
    std::string fontName = "Arimo_Ittalic.ttf";
    if(!font.loadFromFile(fontName)) {
        std::cout << "ERROR: font -> " << fontName;
        exit(EXIT_FAILURE);
    }


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
                }
            }
           
        }
        
        window.clear(sf::Color::Black); // Clear with black background
        sf::Vertex vertices[2] =
        {
            sf::Vertex(sf::Vector2f(10.f, 50.f), sf::Color::Red, sf::Vector2f(100.f, 100.f)),
            sf::Vertex(sf::Vector2f(10.f, 25.f), sf::Color::Red, sf::Vector2f(100.f, 100.f))
        };

        window.draw(vertices, 2, sf::Lines);
        window.display();
    }

    return 0;
}
