#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <limits>

#include "ball.h"
#include "polygon.h"
#include "utils.h"
#include "audio.h"


class Engine {
public:
    std::vector<Ball> balls;
    std::vector<Polygon> polygons;
    std::string winner;
    std::string singer;

    sf::Vector2f sideBounds;
    sf::RenderWindow& window;
    sf::View view;
    sf::Font font;
    sf::Vector2f viewVelocity;

    AudioPlayer& speaker;

    float timeCountdown = 3;
    float timeStart = 0;
    float timeWin;
    float endBannerHeight = -180;

    bool startSequence = true;
    bool hasWinner = false;

    Engine(sf::RenderWindow& window, AudioPlayer& speaker) 
    : window(window)
    , speaker(speaker)
    {
        view = window.getView();
        speaker.playAudio("countdown");
        font.loadFromFile("fonts/arial.ttf");
    }

    void draw(sf::RenderWindow& window, float elapsed_t) {
        // End Banner
        if (view.getCenter().y + view.getSize().y / 2 <= endBannerHeight) {
            sf::RectangleShape bannerWhite(sf::Vector2f(27.0f, 9.0f));
            bannerWhite.setFillColor(sf::Color::White);
            bannerWhite.setPosition(-13.5f, -189.f);
            window.draw(bannerWhite);

            sf::RectangleShape blackSquare(sf::Vector2f(1.5f, 1.5f));
            blackSquare.setFillColor(sf::Color::Black);
            for (int y = 0; y < 6; y++){
                for (int x = 0; x < 18; x++){
                    if ((x + y) % 2 == 0) {
                        blackSquare.setPosition(-13.5f + x * 1.5f, -189.f + y * 1.5f);
                        window.draw(blackSquare);
                    }
                }
            }
        }

        for (auto& ball : balls) ball.draw(window, ball.character == singer);
        for (auto& polygon: polygons) polygon.draw(window);

        // Draw Winner
        if (hasWinner) {
            sf::Texture outerTexture, innerTexture;
            outerTexture.loadFromFile("pictures/winner.png");
            innerTexture.loadFromFile("pictures/" + winner + ".png");

            sf::CircleShape bannerCircle(10), winnerCircle(6);
            bannerCircle.setTexture(&outerTexture);
            winnerCircle.setTexture(&innerTexture);

            float rotationAngle = (elapsed_t - timeWin) * 180 / M_PI;

            bannerCircle.setOrigin(10, 10);
            bannerCircle.setPosition(0, -212);
            bannerCircle.setRotation(rotationAngle);
            bannerCircle.setScale(-1, 1);

            winnerCircle.setOrigin(6, 6);
            winnerCircle.setPosition(0, -212);
            winnerCircle.setRotation(rotationAngle);
            winnerCircle.setScale(-1, 1);

            window.draw(bannerCircle);
            window.draw(winnerCircle);
        }
    }

    void addBall(const Ball& ball) {
        balls.push_back(ball);
    }

    void addPolygon(std::vector<sf::Vector2f> vertices) {
        polygons.push_back(Polygon(vertices));
    }

    void handleBallInteraction(Ball& ball1, Ball& ball2, float dt){

        float minDist = ball1.radius + ball2.radius;
        float d = lengthSquared2f(ball2.position - ball1.position);

        if (d < minDist * minDist) {
            sf::Vector2f n_hat = normalize2f(ball2.position - ball1.position);
            sf::Vector2f t_hat = perp2f(n_hat);

            // Assume Ball 1 and Ball 2 have the same physical properties
            float r = ball1.radius;
            float m = ball1.mass;
            float I = ball1.moment_of_inertia;

            float w1 = ball1.angularVelocity, w2 = ball2.angularVelocity;
            sf::Vector2f v1 = ball1.velocity, v2 = ball2.velocity;
            
            float u = ball1.friction;
            float e = ball1.restitution;

            sf::Vector2f r1 = n_hat * r, r2 = -n_hat * r;
            sf::Vector2f r1p = perp2f(r1), r2p = perp2f(r2);

            sf::Vector2f v_rel = (v2 + w2 * r2p) - (v1 + w1 * r1p);
            float v_n = dot2f(v_rel, n_hat);
            float v_t = dot2f(v_rel, t_hat);

            // Use cross products for angular terms
            float r1cn = cross2f(r1, n_hat);
            float r2cn = cross2f(r2, n_hat);
            float invM_n = 1 / (2/m + (r1cn * r1cn + r2cn * r2cn) / I);
            float J_n = -(1 + e) * v_n * invM_n;

            float r1ct = cross2f(r1, t_hat);
            float r2ct = cross2f(r2, t_hat);
            float invM_t = 1 / (2/m + (r1ct * r1ct + r2ct * r2ct) / I);
            float J_t = -v_t * invM_t;

            J_t = std::clamp(J_t, -u * J_n, u * J_n);

            sf::Vector2f dv = J_n * n_hat + J_t * t_hat;
            ball1.velocity -= dv / m;
            ball2.velocity += dv / m;

            ball1.angularVelocity += cross2f(r1, dv) / I;
            ball2.angularVelocity -= cross2f(r2, dv) / I;

            float sep = 0.5 * (minDist - std::sqrt(d));
            ball1.position += -n_hat * sep;
            ball2.position += n_hat * sep;
        }
    }

    void handleBallPolygonInteraction(Ball& ball, Polygon& polygon) {
        // Check if bounding boxes intersect first
        if ((ball.position.x + ball.radius < polygon.xBound[0] || polygon.xBound[1] < ball.position.x - ball.radius) || 
            (ball.position.y + ball.radius < polygon.yBound[0] || polygon.yBound[1] < ball.position.y - ball.radius)) {
            return;
        }
        
        float minDistSquared = ball.radius * ball.radius;
        sf::Vector2f closestPoint;
        bool collisionDetected = false;

        // Loop through each edge of the polygon
        for (int i = 0; i < polygon.vertices.size(); ++i) {
            sf::Vector2f v1 = polygon.vertices[i];
            sf::Vector2f v2 = polygon.vertices[(i + 1) % polygon.vertices.size()];

            sf::Vector2f edge = v2 - v1;
            sf::Vector2f ballToV1 = ball.position - v1;

            float t = dot2f(ballToV1, edge) / lengthSquared2f(edge);
            t = std::clamp(t, 0.0f, 1.0f);

            sf::Vector2f closest = v1 + t * edge;

            float distSquared = lengthSquared2f(closest - ball.position);

            if (distSquared < minDistSquared) {
                minDistSquared = distSquared;
                closestPoint = closest;
                collisionDetected = true;
            }
        }
        if (!collisionDetected) return;

        sf::Vector2f n_hat = normalize2f(ball.position - closestPoint);
        sf::Vector2f t_hat = perp2f(n_hat);

        float d = std::sqrt(minDistSquared);
        float penetrationDepth = ball.radius - d; 

        sf::Vector2f r_ball = closestPoint - ball.position;
        sf::Vector2f v_rel = ball.velocity + ball.angularVelocity * perp2f(r_ball);

        float v_n = dot2f(v_rel, n_hat);
        float v_t = dot2f(v_rel, t_hat); 
        float e = ball.restitution;
        float u = ball.friction;
        float I = ball.moment_of_inertia;

        float rcn = cross2f(r_ball, n_hat);
        float invM_n = 1 / (1 / ball.mass + (rcn * rcn) / I);
        float J_n = -(1 + e) * v_n * invM_n;

        float rct = cross2f(r_ball, t_hat);
        float invM_t = 1 / (1 / ball.mass + (rct * rct) / I);
        float J_t = -v_t * invM_t;

        J_t = std::clamp(J_t, -u * J_n, u * J_n);
        sf::Vector2f impulse = J_n * n_hat + J_t * t_hat;

        ball.velocity += impulse / ball.mass;
        ball.angularVelocity -= cross2f(r_ball, impulse) / I;

        ball.position += n_hat * penetrationDepth;
    }

    void update(float dt, float elapsed_t) {
        speaker.update();

        for (int i = 0; i < balls.size(); i++){
            Ball& ball1 = balls[i];
            for (int j = i + 1; j < balls.size(); j++) {
                Ball& ball2 = balls[j];
                handleBallInteraction(ball1, ball2, dt);
            }

            for (auto& poly : polygons){
                // Yea its not the most efficient
                handleBallPolygonInteraction(ball1, poly);
            }
        }

        if (elapsed_t < timeCountdown) {
            int number = std::ceil(timeCountdown - elapsed_t);
            sf::Text text(std::to_string(number), font, 128);
            text.setFillColor(sf::Color::Black);
            text.setStyle(sf::Text::Bold);
            text.setPosition(-1.6f, 15.f);
            text.setScale(0.05f, -0.05f);
            window.draw(text);

            for (auto& ball : balls) {
                if (ball.position.y < ball.radius){
                    ball.position.y = ball.radius;
                    ball.velocity.y *= -1;
                }
            }

            sf::RectangleShape startLine(sf::Vector2f(27.0f, 0.1f));
            startLine.setFillColor(sf::Color::Black);
            startLine.setOrigin(startLine.getSize().x / 2, startLine.getSize().y / 2);
            startLine.setPosition(0, 0);
            window.draw(startLine);
        } else if (startSequence){
            timeStart = elapsed_t;
            startSequence = false;
        }

        for (auto& ball : balls) {
            ball.update(dt);
            if (ball.position.y - ball.radius < endBannerHeight && !hasWinner) {
                hasWinner = true;
                winner = ball.character;
                timeWin = elapsed_t;
                speaker.playAudio("congratulations");
            }
        }

        sf::Vector2f viewCenter = view.getCenter();
        if (!startSequence) {
            float minY = std::numeric_limits<float>::max();
            Ball* lowestBall = nullptr;
            for (auto& ball : balls) {
                if (ball.position.y < minY) {
                    minY = ball.position.y;
                    lowestBall = &ball;
                }
            }

            if (!hasWinner){
                speaker.setSinger(lowestBall->character, elapsed_t - timeStart);
                singer = lowestBall->character;
                viewVelocity.y = lowestBall->position.y - viewCenter.y;
            }
            else viewVelocity.y = -200 - viewCenter.y;
            
            viewCenter += dt * viewVelocity;
            view.setCenter(viewCenter);
            window.setView(view);
        }
    }
};


#endif 
