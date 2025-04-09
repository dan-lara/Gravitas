/**
 * @file Spaceship.hpp
 * @author Gravitas
 *
 * @brief Definition of the Spaceship class.
 * @details This class represents a spaceship in a 2D space simulation game.
 */

// #pragma once
#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "utils.hpp"
#include "GravitySource.hpp"

#define SHIP_PATH "src/assets/imgs/ship.png"
#define SHIP_CRASH_PATH "src/assets/imgs/ship_crash.png"
#define SHIP_SCALE 0.3f
#define SHIP_RADIUS 8.f
#define SHIP_COLOR sf::Color::White

struct FuelParticle {
    sf::Vector2f pos; // Position actuelle de la particule
    sf::Vector2f vel; // Vitesse
    float lifetime;
    sf::Color color; // Coleur simple

    FuelParticle(sf::Vector2f p, sf::Vector2f v)
        : pos(p), vel(v), lifetime(0.5f), color(sf::Color::Red) {}
};

class Spaceship {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f pos0;

    sf::CircleShape shape;
    sf::Texture spaceshipTexture;
    sf::Texture spaceshipTextureCrash;
    sf::Sprite sprite;

    std::vector<sf::Vector2f> trail; 
    bool active;
    bool hasFailed = false;
    std::vector<FuelParticle> fuelParticles;
    bool boosting = false;
    bool spriteLoaded = false;

public:
    Spaceship(float pos_x, float pos_y);
    Spaceship(float pos_x, float pos_y, float size);
    Spaceship(float pos_x, float pos_y, std::string texturePath, float scale = SHIP_SCALE);

    void setVelocity(sf::Vector2f velocity);

    void updatePhysics(std::vector<GravitySource> &sources, float dt);

    void render(sf::RenderWindow &window);

    // Prédire la trajectoire
    std::vector<sf::Vector2f> predictTrajectory(
        const std::vector<GravitySource>& sources,
        sf::Vector2f initialVelocity,
        int steps = 200,
        float dt = 1.0f
    ) const;

    // Accélération vers l'avant
    void boost1();

    // Accélération latérale
    void boost2();

    // Mettre à jour les particules
    void updateParticles(float dt);
    void reset();

    sf::Vector2f getPos() { return pos; }
    bool isActive() { return active; }
    bool getFailureStatus() const { return hasFailed; }
    void clearFailureStatus() { hasFailed = false; }

};

#endif // SPACESHIP_HPP

