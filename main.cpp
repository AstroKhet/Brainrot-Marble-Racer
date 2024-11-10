#include <SFML/Graphics.hpp>

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

#include "audio.h"
#include "ball.h"
#include "polygon.h"
#include "engine.h"
#include "scene.h"

int main()
{
    // Render Window
    const float W_WIDTH = 540;
    const float W_HEIGHT = 960;
    const float scale = 0.05f;
    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "KSI Marble Race");
    sf::Vector2f viewCenter(0, 0);
    sf::View view(viewCenter, sf::Vector2f(W_WIDTH * scale, -W_HEIGHT * scale));
    window.setView(view);

    // Engine
    AudioPlayer speaker;
    Engine engine(window, speaker);

    // Scene
    std::vector<Ball> balls;
    std::vector<Polygon> polygons;
    loadScene(engine, balls, polygons);

    // Time variables
    double dt, elapsed_t = 0.0;
    std::chrono::steady_clock::time_point last_t = std::chrono::steady_clock::now();

    while (window.isOpen())
    {   
        double dt = std::chrono::duration<double>(std::chrono::steady_clock::now() - last_t).count();
        last_t = std::chrono::steady_clock::now();
        elapsed_t += dt;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(135, 206, 235));  // Sky blue

        engine.update(dt, elapsed_t);
        engine.draw(window, elapsed_t);

        window.display();
    }

    return 0;
}