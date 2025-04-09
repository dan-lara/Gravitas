/**
 * @file GravitySource.cpp
 * @author Gravitas
 *
 * @brief Implementation of the GravitySource class.
 */


#include "include/GravitySource.hpp"

#include <cmath> 

GravitySource::GravitySource(sf::Vector2f center, float radius, float strength, float angularSpeed)
    : center(center), radius(radius), strength(strength), angularSpeed(angularSpeed), texture(SOURCE_PATH), sprite(texture) {
        shape.setPosition(center);
        shape.setRadius(radius);
        shape.setOrigin(sf::Vector2f(radius, radius)); // SFML 3.0 : setOrigin ne peut être utilisé qu'avec Vector2f
        shape.setFillColor(color);
        update(0); // Mise à jour initiale de la position
}

GravitySource::GravitySource(sf::Vector2f center, float radius, float strength, float angularSpeed, sf::Color color)
    : center(center), radius(radius), strength(strength), angularSpeed(angularSpeed), color(color), texture(SOURCE_PATH), sprite(texture) {
        shape.setPosition(center);
        shape.setRadius(radius);
        shape.setOrigin(sf::Vector2f(radius, radius)); // SFML 3.0 : setOrigin ne peut être utilisé qu'avec Vector2f
        shape.setFillColor(color);
        update(0); // Mise à jour initiale de la position
}

GravitySource::GravitySource(sf::Vector2f center, float radius, float strength, float angularSpeed, bool useSprite, std::string texturePath, float scale)
    : center(center), radius(radius), strength(strength), angularSpeed(angularSpeed)
    , texture(texturePath), sprite(texture), useSprite(useSprite) {
        if (useSprite) {
            if (!texture.loadFromFile(texturePath)) {
                std::cerr << "Failed to load texture\n";
            }
            spriteLoaded = true;
            sprite.setTexture(texture);
            sprite.setOrigin({float(texture.getSize().x) / 2, float(texture.getSize().y) / 2});
            sprite.setPosition(center);
            sprite.setScale({scale, scale});
            sprite.setPosition(center);
            radius = std::max(radius, std::max(texture.getSize().x, texture.getSize().y) * scale / 2.f); // Ajuster le rayon en fonction de la texture
            update(0); // Mise à jour initiale de la position

        } else {
            GravitySource(center, radius, strength, angularSpeed);
        }
}

void GravitySource::update(float dt) {
    angle += angularSpeed * dt;
    float rad = angle * PI / 180.f;
    sf::Vector2f pos = center + sf::Vector2f(std::cos(rad) * radius, std::sin(rad) * radius);
    shape.setPosition(pos);
}

void GravitySource::render(sf::RenderWindow &window) {
    if (spriteLoaded) {
        window.draw(sprite);
    } else {
        shape.setFillColor(color);
        window.draw(shape);
    }
}