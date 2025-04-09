/**
 * @file Destination.hpp
 * @author Gravitas
 *
 * @brief Definition of the Destination class.
 */

#ifndef DESTINATION_HPP
#define DESTINATION_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Spaceship.hpp"

class Destination {
    sf::Vector2f pos;
    sf::CircleShape shape;
    int radius = 50;

public:
    Destination(float x, float y);

    void render(sf::RenderWindow &window);

    bool checkArrival(Spaceship &ship);
};

#endif // DESTINATION_HPP
