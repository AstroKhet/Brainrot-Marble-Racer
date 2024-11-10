#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <sstream>
#include <filesystem>
#include <string>
#include "ball.h"
#include "polygon.h"
#include "engine.h"


inline void loadScene(Engine& engine, std::vector<Ball>& balls, std::vector<Polygon>& polygons) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(-5.0f, 5.0f);
    std::uniform_real_distribution<float> distY(0.0f, 5.0f);

    for (const auto& pngFile : std::filesystem::directory_iterator("pictures/144p")) {
        std::string character = pngFile.path().stem().string();
        Ball marble(sf::Vector2f(distX(gen), distY(gen)), character);
        engine.addBall(marble);
    }

    std::ifstream polygonData("map.txt");
    std::string str;
    std::vector<sf::Vector2f> vertices;

    while (std::getline(polygonData, str)) {
        if (str.empty()) {
            engine.addPolygon(vertices);
            vertices.clear();
            continue;
        }
        else if (str[0] == '#') continue;

        std::istringstream lineStream(str);
        float x, y;
        if (lineStream >> x >> y) {
            vertices.emplace_back(x, y);
        }
    }

    if (!vertices.empty()) engine.addPolygon(vertices);
}


#endif
