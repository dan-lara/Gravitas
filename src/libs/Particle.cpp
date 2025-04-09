/**
 * @file Particle.cpp
 * @author Gravitas
 *
 * @brief Implementation of the Particle class.
 */

#include "include/Particle.hpp"

Particle::Particle(float pos_x, float pos_y, float vel_x, float vel_y, float radius, sf::Color color)
    : pos(pos_x, pos_y), vel(vel_x, vel_y), shape(radius), color(sf::Color::White)
{
    pos.x = pos_x;
    pos.y = pos_y;

    vel.x = vel_x;
    vel.y = vel_y;

    shape.setPosition(pos);
    shape.setFillColor(color);
    shape.setRadius(radius);
}

void Particle::set_color(sf::Color color)
{
    shape.setFillColor(color);
}

void Particle::render(sf::RenderWindow& window)
{
    shape.setPosition(pos);
    window.draw(shape);
}

void Particle::updatePhysics(GravitySource &source)
{
    float distance_x = source.getPos().x - pos.x; 
    float distance_y = source.getPos().y - pos.y; 

    float distance = sqrt(distance_x * distance_x + distance_y * distance_y); 

    float inverse_distance = 1.f / distance;   

    float normalized_x = distance_x * inverse_distance; 
    float normalized_y = distance_y * inverse_distance;

    float acceleration_x = normalized_x * source.getStrength() * inverse_distance * inverse_distance; 
    float acceleration_y = normalized_y * source.getStrength() * inverse_distance * inverse_distance;

    vel.x += acceleration_x;
    vel.y += acceleration_y;

    pos.x += vel.x;
    pos.y += vel.y;
}