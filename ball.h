#ifndef BALL_H
#define BALL_H


#include <SFML/Graphics.hpp>
#include <cmath>


class Ball {
public:
    std::string character = "";
    std::string characterImage;

    sf::CircleShape circle;
    sf::Texture texture;
    sf::Vector2f position = {0.0f, 0.0f};
    sf::Vector2f velocity = {0.0f, 0.0f};
    sf::Vector2f acceleration = {0.0f, -15.f};

    float angularDisplacement = 0.0f;
    float angularVelocity = 0.0f;
    float angularAcceleration = 0.0f;
    float mass = 1.0f;
    float radius = 1.0f;
    float friction = 0.2f;
    float restitution = 0.67f;
    float moment_of_inertia = 0.5f * M_PI * radius * radius;

    bool singing = false;

    Ball(sf::Vector2f pos, std::string character)
    : position(pos), character(character)
    {   
        circle.setRadius(radius);
        circle.setOutlineThickness(-radius / 10.f);
        characterImage = "pictures/144p/" + character + ".png";
        texture.loadFromFile(characterImage); 
    }

    void setSinging(bool status) {
        singing = status;
    }

    void draw(sf::RenderWindow& window, bool isSinger) {
        circle.setOrigin(radius, radius);
        circle.setPosition(position);
        circle.setRotation(angularDisplacement * 180.0f / M_PI + 180.0f);
        circle.setTexture(&texture);
        circle.setOutlineColor(isSinger ? sf::Color::Green : sf::Color::Black);
        window.draw(circle);
    }

    void update(float dt) {
        velocity += acceleration * dt;
        position += velocity * dt;
        angularVelocity += angularAcceleration * dt;
        angularDisplacement += angularVelocity * dt;
    }
};



#endif

