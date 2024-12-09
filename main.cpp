#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include <random>
#include <vector>


struct Edge {
    std::size_t startVertexIndex;
    std::size_t endVertexIndex;
};

// Check if a point is near a vertex
std::size_t findVertex(const sf::Vector2f& mousePosition, const std::vector<sf::CircleShape>& vertices, float radius = 10.f) {
    for (std::size_t i = 0; i < vertices.size(); ++i) {
        sf::Vector2f vertexPosition = vertices[i].getPosition() + sf::Vector2f(vertices[i].getRadius(), vertices[i].getRadius());
        float distance = std::hypot(mousePosition.x - vertexPosition.x, mousePosition.y - vertexPosition.y);
        if (distance <= radius) {
            return i;
        }
    }
    return static_cast<std::size_t>(-1); // Return invalid index if no vertex is found
}

// Function to add a vertex at the mouse position
void addVertex(const sf::Vector2f& mousePosition, const sf::Color& color,
               std::vector<sf::CircleShape>& vertices) {
    sf::CircleShape vertex(5.f); // Radius of the vertex
    vertex.setFillColor(color);  // Set the vertex color
    vertex.setPosition(mousePosition.x - vertex.getRadius(),
                        mousePosition.y - vertex.getRadius()); // Center the vertex at mouse position
    vertices.push_back(vertex);
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Vertex Connections");
    std::vector<sf::CircleShape> vertices;
    std::vector<Edge> edges;

    bool isMousePressed = false;
    sf::Color currentColor = sf::Color::Red;

    // Variables to track vertex selection for edge creation
    std::size_t selectedVertexIndex = static_cast<std::size_t>(-1);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //--Mouse Handling--
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (!isMousePressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

                    // Check if clicking on an existing vertex
                    std::size_t clickedVertexIndex = findVertex(worldPosition, vertices);
                    if (clickedVertexIndex != static_cast<std::size_t>(-1)) {
                        if (selectedVertexIndex == static_cast<std::size_t>(-1)) {
                            // First vertex selected
                            selectedVertexIndex = clickedVertexIndex;
                        } else {
                            // Second vertex selected, create edge
                            edges.push_back({selectedVertexIndex, clickedVertexIndex});
                            selectedVertexIndex = static_cast<std::size_t>(-1); // Reset selection
                        }
                    } else {
                        // Add a new vertex
                        addVertex(worldPosition, currentColor, vertices);
                    }
                    isMousePressed = true;
                }
            } else {
                isMousePressed = false;
            }

            // Key Handling
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::C) {
                    vertices.clear();
                    edges.clear();
                }
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }

        // Draw
        window.clear(sf::Color::Black);

        // Draw edges
        for (const auto& edge : edges) {
            sf::Vertex line[] = {
                sf::Vertex(vertices[edge.startVertexIndex].getPosition() + sf::Vector2f(5.f, 5.f), sf::Color::White),
                sf::Vertex(vertices[edge.endVertexIndex].getPosition() + sf::Vector2f(5.f, 5.f), sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }

        // Draw vertices
        for (const auto& vertex : vertices) {
            window.draw(vertex);
        }

        window.display();
    }

    return 0;
}
