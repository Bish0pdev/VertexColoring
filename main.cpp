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
    sf::Color currentColor = sf::Color::Red;

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
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePosition(event.mouseButton.x, event.mouseButton.y);
            sf::Vertex vertex(mousePosition, currentColor);
            vertices.push_back(vertex);
        }

        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::R) currentColor = sf::Color::Red;
            if (event.key.code == sf::Keyboard::G) currentColor = sf::Color::Green;
            if (event.key.code == sf::Keyboard::B) currentColor = sf::Color::Blue;
            if (event.key.code == sf::Keyboard::C) vertices.clear();
            if (event.key.code == sf::Keyboard::Z && !vertices.empty()) vertices.pop_back();
        }
    }

    window.clear(sf::Color::Black);

    if (!vertices.empty())
    {
        window.draw(&vertices[0], vertices.size(), sf::LineStrip);
    }

    window.display();
    }


    return 0;
}
