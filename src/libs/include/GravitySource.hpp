/**
 * @file GravitySource.hpp
 * @author Gravitas
 *
 * @brief Implementation of the GravitySource class.
 * @details This class represents a gravity source in a 2D space simulation game.
 */

#ifndef GRAVITY_SOURCE_HPP
#define GRAVITY_SOURCE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "utils.hpp"

#define SOURCE_PATH "src/assets/imgs/spaceship.png"
#define SOURCE_SCALE 1.f
#define SOURCE_RADIUS 8.f
// #define SOURCE_COLOR sf::Color::Black
#define SOURCE_COLOR sf::Color(169, 169, 169)

class GravitySource {
    sf::Vector2f center;      // Centre de rotation
    float radius = SOURCE_RADIUS;             // Rayon de l'orbite
    float strength;
    float angle = 0.f;        // Angle actuel (unité : degrés)
    float angularSpeed;       // Vitesse de rotation (degrés/seconde)
    sf::CircleShape shape;
    sf::Color color = SOURCE_COLOR;

    sf::Texture texture;
    sf::Sprite sprite;
    bool useSprite = false;
    bool spriteLoaded = false;

public:
    GravitySource(sf::Vector2f center, float radius, float strength, float angularSpeed);
    GravitySource(sf::Vector2f center, float radius, float strength, float angularSpeed, sf::Color color);
    GravitySource(sf::Vector2f center, float radius, float strength, float angularSpeed, bool useSprite, std::string texturePath = SOURCE_PATH, float scale = SOURCE_SCALE);

    void update(float dt);

    void render(sf::RenderWindow &window);

    sf::Vector2f getPos() const {
        return shape.getPosition();
    }

    float getStrength() const {
        return strength;
    }
    float getRadius() const {
        return radius;
    }
    float getAngle() const {
        return angle;
    }
};
#endif // GRAVITY_SOURCE_HPP
