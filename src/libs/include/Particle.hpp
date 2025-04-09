/**
 * @file Particle.hpp
 * @author Gravitas
 *
 * @brief Definition of the Particle class.
 * @details
 * Its the class that represents a particle in the simulation.
 * The most basic one.
 */

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>
#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>

#include "GravitySource.hpp"

class Particle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape shape;
    sf::Color color;
    float radius;
 
 
public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y, float radius = 5.f, sf::Color color = sf::Color::White);

    void set_color(sf::Color color);
 
    void render(sf::RenderWindow& window);

    
    void updatePhysics(GravitySource &source);
 
};

#endif // PARTICLE_HPP
