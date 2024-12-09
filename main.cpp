#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include <random>
#include <vector>


// Function to add a vertex at the mouse position
void addVertex(const sf::Vector2f& mousePosition, const sf::Color& color,
               std::vector<sf::CircleShape>& vertices) {
    sf::CircleShape vertex(5.f); // Radius of the vertex
    vertex.setFillColor(color);  // Set the vertex color
    vertex.setPosition(mousePosition.x - vertex.getRadius(),
                        mousePosition.y - vertex.getRadius()); // Center the vertex at mouse position
    vertices.push_back(vertex);
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Test");
    std::vector<sf::CircleShape> vertices;
   // std::vector<Edge> edges;

    const float fixedTimeStep = 1.0f / 60.0f; // 60 updates per second
    float accumulator = 0.0f;                // Tracks unprocessed time
    sf::Clock clock;
    

    //Load font(s)
    sf::Font font;
    std::string fontName = "Arimo_Ittalic.ttf";
    if(!font.loadFromFile(fontName)) {
        std::cout << "ERROR: font -> " << fontName;
        exit(EXIT_FAILURE);
    }
    sf::Color currentColor = sf::Color::Red;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //--Mouse Handling--
            bool isMousePressed = false;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (!isMousePressed) {
                    std::cout << "Mouse button pressed" << std::endl;
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

                    addVertex(worldPosition, sf::Color::Red, vertices);
                    isMousePressed = true;
                }
            } else {
                isMousePressed = false;
            }

            if(isMousePressed) {
                
            }
            //----
            if (event.type == sf::Event::KeyReleased) //input
            {
                if (event.key.code == sf::Keyboard::C) vertices.clear();
                if (event.key.code == sf::Keyboard::Z && !vertices.empty()) vertices.pop_back();
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }

        //Start of game/Update loop
        window.clear(sf::Color::Black); 

        //Draw Vertices
        for (const auto& vertex : vertices)
        {
            window.draw(vertex);
        }

        //Fixed Time Set
        float deltaTime = clock.restart().asSeconds();
        accumulator += deltaTime;
        while (accumulator >= fixedTimeStep)
        {
            accumulator -= fixedTimeStep;
        }
        
        window.display();
    }

    return 0;
}
