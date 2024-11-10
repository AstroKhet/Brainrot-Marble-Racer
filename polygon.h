#ifndef POLY_H
#define POLY_H

#include <SFML/Graphics.hpp>

#include <cmath>
#include <vector>
#include <limits>

class Polygon {
public:
    std::vector<sf::Vector2f> vertices;

    float yBound[2] = {std::numeric_limits<float>::max(), std::numeric_limits<float>::min()};
    float xBound[2] = {std::numeric_limits<float>::max(), std::numeric_limits<float>::min()};

    Polygon(std::vector<sf::Vector2f> vertices)
        : vertices(vertices) 
        {
            for (auto& v : vertices) {
                yBound[0] = std::min(v.y, yBound[0]);
                yBound[1] = std::max(v.y, yBound[1]);

                xBound[0] = std::min(v.x, xBound[0]);
                xBound[1] = std::max(v.x, xBound[1]);
            }
        }

    void update(float dt) {
        // Polygons don't move... yet
    }

    void draw(sf::RenderWindow& window) {
        sf::ConvexShape shape;
        shape.setPointCount(vertices.size());
        for (int i = 0; i < vertices.size(); i++) {
            shape.setPoint(i, vertices[i]);
        }
        shape.setFillColor(sf::Color::Black);
        window.draw(shape);
    }
};


#endif