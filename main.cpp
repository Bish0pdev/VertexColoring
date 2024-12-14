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
std::vector<std::size_t> colorGraphGreedy(const std::map<std::size_t, std::set<std::size_t>>& adjacencyList, std::size_t numVertices) {
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


//-- Backtracking Methods
bool isColorValid(const std::map<std::size_t, std::set<std::size_t>>& adjacencyList,
                  const std::vector<std::size_t>& vertexColors,
                  std::size_t vertex,
                  std::size_t color) {
    for (std::size_t neighbor : adjacencyList.at(vertex)) {
        if (vertexColors[neighbor] == color) {
            return false; // Conflict with an adjacent vertex
        }
    }
    return true;
}

bool colorGraphBacktracking(const std::map<std::size_t, std::set<std::size_t>>& adjacencyList,
                            std::vector<std::size_t>& vertexColors,
                            std::size_t vertex,
                            std::size_t maxColors) {
    if (vertex == vertexColors.size()) {
        return true; // All vertices are successfully colored
    }

    // Try each color
    for (std::size_t color = 0; color < maxColors; ++color) {
        if (isColorValid(adjacencyList, vertexColors, vertex, color)) {
            vertexColors[vertex] = color; // Assign the color

            // Recursively color the next vertex
            if (colorGraphBacktracking(adjacencyList, vertexColors, vertex + 1, maxColors)) {
                return true; // Found a solution
            }

            vertexColors[vertex] = -1; // Backtrack
        }
    }

    return false; // No valid coloring found
}

std::vector<std::size_t> colorGraphUsingBacktracking(const std::map<std::size_t, std::set<std::size_t>>& adjacencyList,
                                                     std::size_t numVertices,
                                                     std::size_t maxColors) {
    std::vector<std::size_t> vertexColors(numVertices, -1); // Initialize all vertices as uncolored
    if (colorGraphBacktracking(adjacencyList, vertexColors, 0, maxColors)) {
        return vertexColors; // Return the coloring solution
    } else {
        return {}; // Return an empty vector if no solution is found
    }
}

//----
const float GRID_SPACING = 50.f;  // Distance between grid lines
const sf::Color GRID_COLOR = sf::Color(50, 50, 50); // Dim gray for grid lines
const sf::Color AXIS_COLOR = sf::Color(200, 200, 200); // Lighter gray for axes
const float LINE_THICKNESS = 1.f; // Thickness of grid lines

void drawGrid(sf::RenderWindow& window, const sf::View& view, sf::Font& font) {
    sf::FloatRect viewBounds(
        view.getCenter() - view.getSize() / 2.f,
        view.getSize()
    );

    // Draw vertical grid lines
    for (float x = std::floor(viewBounds.left / GRID_SPACING) * GRID_SPACING;
         x < viewBounds.left + viewBounds.width; x += GRID_SPACING) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, viewBounds.top), GRID_COLOR),
            sf::Vertex(sf::Vector2f(x, viewBounds.top + viewBounds.height), GRID_COLOR)
        };
        window.draw(line, 2, sf::Lines);
    }

    // Draw horizontal grid lines
    for (float y = std::floor(viewBounds.top / GRID_SPACING) * GRID_SPACING;
         y < viewBounds.top + viewBounds.height; y += GRID_SPACING) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(viewBounds.left, y), GRID_COLOR),
            sf::Vertex(sf::Vector2f(viewBounds.left + viewBounds.width, y), GRID_COLOR)
        };
        window.draw(line, 2, sf::Lines);
    }

    // Draw x-axis (horizontal) and y-axis (vertical)
    sf::Vertex xAxis[] = {
        sf::Vertex(sf::Vector2f(viewBounds.left, 0.f), AXIS_COLOR),
        sf::Vertex(sf::Vector2f(viewBounds.left + viewBounds.width, 0.f), AXIS_COLOR)
    };
    window.draw(xAxis, 2, sf::Lines);

    sf::Vertex yAxis[] = {
        sf::Vertex(sf::Vector2f(0.f, viewBounds.top), AXIS_COLOR),
        sf::Vertex(sf::Vector2f(0.f, viewBounds.top + viewBounds.height), AXIS_COLOR)
    };
    window.draw(yAxis, 2, sf::Lines);
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Vertex Coloring");
    sf::View view(sf::FloatRect(0.f, 0.f, 800.f, 600.f)); // Default view
    std::vector<sf::CircleShape> vertices;
    std::vector<Edge> edges;

    
    bool physicsEnabled = false;
    bool isMousePressed = false;
    bool isConnecting = false;
    bool isDragging = false;
    float zoomLevel = 1.f;

    bool backtracking = false;

    sf::Color backgroundColor;
    sf::Vector2i initialMousePosition;
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
    vertexCountText.setString("Vertices: ");
    sf::Text colorCountText;
    colorCountText.setFont(font);
    colorCountText.setCharacterSize(20);
    colorCountText.setFillColor(sf::Color::White);
    colorCountText.setPosition(10.f, 40.f);
    colorCountText.setString("Colors: ");

    sf::Text solverModeText;
    solverModeText.setFont(font);
    solverModeText.setCharacterSize(20);
    solverModeText.setFillColor(sf::Color::White);
    solverModeText.setPosition(10.f,70.f);
    

    // Variable to store current coloring
    std::vector<std::size_t> currentColors;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //--Mouse Handling--
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    zoomLevel *= 0.9f; // Zoom in (reduce view size)
                } else if (event.mouseWheelScroll.delta < 0) {
                    zoomLevel *= 1.1f; // Zoom out (increase view size)
                }
                view.setSize(window.getDefaultView().getSize() * zoomLevel);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                if (!isDragging) {
                    isDragging = true;
                    initialMousePosition = sf::Mouse::getPosition(window);
                } else {
                    sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f delta = window.mapPixelToCoords(initialMousePosition) -
                                         window.mapPixelToCoords(currentMousePosition);
                    view.move(delta);
                    initialMousePosition = currentMousePosition; // Update for continuous dragging
                }
            } else {
                isDragging = false;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (!isMousePressed) {
                    // Handle vertex and edge logic
                    sf::Vector2f worldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
                    std::size_t clickedVertexIndex = findVertex(worldPosition, vertices);
                    if (clickedVertexIndex != static_cast<std::size_t>(-1)) {
                        if (selectedVertexIndex == static_cast<std::size_t>(-1)) {
                            selectedVertexIndex = clickedVertexIndex;
                            isConnecting = true;
                        } else {
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
                    auto adjacencyList = buildAdjacencyList(edges, vertices.size());
                    if(backtracking) {
                        currentColors = colorGraphUsingBacktracking(adjacencyList, vertices.size(), 6);
                    } else {
                    currentColors = colorGraphGreedy(adjacencyList, vertices.size());
                    }
                    vertexCountText.setString("Vertices: " + std::to_string(vertices.size()));
                    std::set<std::size_t> uniqueColors(currentColors.begin(), currentColors.end());
                    colorCountText.setString("Colors: " + std::to_string(uniqueColors.size()));
                }
                if (event.key.code == sf::Keyboard::M) {
                    backtracking = !backtracking;
                }
                if (event.key.code == sf::Keyboard::P) {
                    physicsEnabled = !physicsEnabled;
                }
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }

        // Update vertex colors
        if (!currentColors.empty()) {
            for (std::size_t i = 0; i < vertices.size(); ++i) {
                vertices[i].setFillColor(getColorByIndex(currentColors[i]));
            }
        }
        // Clear and set view
        window.clear(backgroundColor);
        window.setView(view);

        drawGrid(window, view, font);

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

        // Draw connecting line
        if (isConnecting && selectedVertexIndex != static_cast<std::size_t>(-1)) {
            sf::Vector2f worldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
            sf::Vertex tempLine[] = {
                sf::Vertex(vertices[selectedVertexIndex].getPosition() + sf::Vector2f(5.f, 5.f), sf::Color::White),
                sf::Vertex(worldPosition, sf::Color::Yellow)
            };
            window.draw(tempLine, 2, sf::Lines);
        }

        if(isConnecting) {
            backgroundColor = sf::Color(40,40,40);
        } else {
            backgroundColor = sf::Color::Black;
        }
        // Draw text
        window.setView(window.getDefaultView()); // Reset view for UI
        window.draw(vertexCountText);
        window.draw(colorCountText);
        
        if(backtracking) {
            solverModeText.setString("Algorithm: Backtracking");
        } else solverModeText.setString("Algorithm: Greedy");
        window.draw(solverModeText);

        window.display();
    }

    return 0;
}
