#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>

#include <cmath>


inline float distanceSquared2f(sf::Vector2f a, sf::Vector2f b){
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline float lengthSquared2f(sf::Vector2f a) {
    return a.x * a.x + a.y * a.y;
}

inline float length2f(sf::Vector2f a){
    return std::sqrt(a.x * a.x + a.y * a.y);
}

inline float dot2f(sf::Vector2f a, sf::Vector2f b){
    return a.x * b.x + a.y * b.y;
}

inline float cross2f(sf::Vector2f a, sf::Vector2f b) {
    // Magnitude of the vector that would result from a regular 3D cross product of the input vectors, taking their Z values implicitly as 0
    return a.y * b.x - a.x * b.y;
}

inline sf::Vector2f perp2f(sf::Vector2f a) {
    return sf::Vector2f(-a.y, a.x);
}

inline sf::Vector2f normalize2f(sf::Vector2f a){
    float length = length2f(a);
    if (length != 0){
        return a / length;
    } else return sf::Vector2f(0, 0);
}

inline void drawCircle(sf::RenderWindow& window, sf::Vector2f pos, float radius, sf::Color color){
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setOrigin(radius, radius);
    circle.setPosition(pos.x, pos.y);
    window.draw(circle);
}


#endif
