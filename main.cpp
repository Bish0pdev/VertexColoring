#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <map>
#include <array>

// Struct for edges
struct Edge {
    std::size_t startVertexIndex;
    std::size_t endVertexIndex;
};

// Function to find a vertex near a mouse click
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

// Build the adjacency list for the graph
std::map<std::size_t, std::set<std::size_t>> buildAdjacencyList(const std::vector<Edge>& edges, std::size_t numVertices) {
    std::map<std::size_t, std::set<std::size_t>> adjacencyList;

    // Initialize the adjacency list with all vertices
    for (std::size_t i = 0; i < numVertices; ++i) {
        adjacencyList[i] = {}; // Ensure every vertex is represented
    }

    // Add connections based on edges
    for (const auto& edge : edges) {
        adjacencyList[edge.startVertexIndex].insert(edge.endVertexIndex);
        adjacencyList[edge.endVertexIndex].insert(edge.startVertexIndex);
    }

    return adjacencyList;
}

// Add a vertex at a given position with a default color
void addVertex(const sf::Vector2f& mousePosition, const sf::Color& color,
               std::vector<sf::CircleShape>& vertices) {
    sf::CircleShape vertex(5.f); // Radius of the vertex
    vertex.setFillColor(color);  // Set the vertex color
    vertex.setPosition(mousePosition.x - vertex.getRadius(),
                        mousePosition.y - vertex.getRadius()); // Center the vertex at mouse position
    vertices.push_back(vertex);
}

// Function to get a distinct color
sf::Color getColorByIndex(std::size_t colorIndex) {
    static std::array<sf::Color, 6> colors = {sf::Color::Blue, sf::Color::Red, sf::Color::Green,
                                              sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};
    return colors[colorIndex % colors.size()];
}

// Function to perform greedy vertex coloring
std::vector<std::size_t> colorVertices(const std::map<std::size_t, std::set<std::size_t>>& adjacencyList, std::size_t numVertices) {
    std::vector<std::size_t> vertexColors(numVertices, -1); // -1 indicates no color assigned
    for (std::size_t vertex = 0; vertex < numVertices; ++vertex) {
        // Collect colors of adjacent vertices
        std::set<std::size_t> forbiddenColors;
        for (std::size_t neighbor : adjacencyList.at(vertex)) {
            if (vertexColors[neighbor] != -1) {
                forbiddenColors.insert(vertexColors[neighbor]);
            }
        }

        // Assign the smallest available color
        std::size_t color = 0;
        while (forbiddenColors.find(color) != forbiddenColors.end()) {
            ++color;
        }
        vertexColors[vertex] = color;
    }
    return vertexColors;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Vertex Coloring");
    std::vector<sf::CircleShape> vertices;
    std::vector<Edge> edges;

    bool isMousePressed = false;
    bool isConnecting = false;

    std::size_t selectedVertexIndex = static_cast<std::size_t>(-1);

    // Font and Text for displaying info
    sf::Font font;
    if (!font.loadFromFile("Arimo.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return 1;
    }

    sf::Text vertexCountText;
    vertexCountText.setFont(font);
    vertexCountText.setCharacterSize(20);
    vertexCountText.setFillColor(sf::Color::White);
    vertexCountText.setPosition(10.f, 10.f);

    sf::Text colorCountText;
    colorCountText.setFont(font);
    colorCountText.setCharacterSize(20);
    colorCountText.setFillColor(sf::Color::White);
    colorCountText.setPosition(10.f, 40.f);


    // Variable to store current coloring
    std::vector<std::size_t> currentColors;

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
         sf::Vector2f worldPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //--Mouse Handling--
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (!isMousePressed) {
                    
                   

                    // Check if clicking on an existing vertex
                    std::size_t clickedVertexIndex = findVertex(worldPosition, vertices);
                    if (clickedVertexIndex != static_cast<std::size_t>(-1)) {
                        if (selectedVertexIndex == static_cast<std::size_t>(-1)) {
                            // First vertex selected
                            selectedVertexIndex = clickedVertexIndex;
                            isConnecting = true;
                        } else {
                            // Second vertex selected, create edge
                            edges.push_back({selectedVertexIndex, clickedVertexIndex});
                            selectedVertexIndex = static_cast<std::size_t>(-1); // Reset selection
                            isConnecting = false;
                        }
                    } else {
                        addVertex(worldPosition, sf::Color::White, vertices);
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
                    currentColors.clear();
                }
                if (event.key.code == sf::Keyboard::Space) {
                    // Recalculate vertex colors
                    auto adjacencyList = buildAdjacencyList(edges, vertices.size());
                    currentColors = colorVertices(adjacencyList, vertices.size());

                    // Update text
                    vertexCountText.setString("Vertices: " + std::to_string(vertices.size()));
                    std::set<std::size_t> uniqueColors(currentColors.begin(), currentColors.end());
                    colorCountText.setString("Colors: " + std::to_string(uniqueColors.size()));
                }
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }

        // Update vertex colors based on current coloring
        if (!currentColors.empty()) {
            for (std::size_t i = 0; i < vertices.size(); ++i) {
                vertices[i].setFillColor(getColorByIndex(currentColors[i]));
            }
        }

        // Draw background
        if(isConnecting) {
           window.clear(sf::Color(40,40,40,1));
        } else
        {
             window.clear(sf::Color::Black);
        }
        

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

        // Draw temporary line if connecting
        if (isConnecting && selectedVertexIndex != static_cast<std::size_t>(-1)) {
            sf::Vertex tempLine[] = {
                sf::Vertex(vertices[selectedVertexIndex].getPosition() + sf::Vector2f(5.f, 5.f), sf::Color::White),
                sf::Vertex(worldPosition, sf::Color::Yellow)
            };
            window.draw(tempLine, 2, sf::Lines);
        }

        // Draw text
        window.draw(vertexCountText);
        window.draw(colorCountText);

        window.display();
    }

    return 0;
}
