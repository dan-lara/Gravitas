/**
 * @file Destination.cpp
 * @author Gravitas
 *
 * @brief Implementation of the Destination class.
 */

#include "include/Destination.hpp"

Destination::Destination(float x, float y) {
    pos = {x, y};
    shape.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius));
    shape.setFillColor(sf::Color::Green);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(5);
    shape.setRadius(radius);
}

bool Destination::checkArrival(Spaceship &ship) {
    sf::Vector2f shipPos = ship.getPos();
    float dist = std::sqrt((pos.x - shipPos.x) * (pos.x - shipPos.x) +
                           (pos.y - shipPos.y) * (pos.y - shipPos.y));
    return dist < radius;
}
void Destination::render(sf::RenderWindow &window) {
    window.draw(shape);
}