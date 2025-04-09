/**
 * @file LevelManager.hpp
 * @author Gravitas
 *
 * @brief Definition of the LevelManager class.
 * @details
 * The LevelManager class is responsible for managing the levels in the game.
 * It creates the game turney and handles the transition between levels.
 */

// #pragma once
#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <optional>

#include "include/GravitySource.hpp"
#include "include/Spaceship.hpp"
#include "include/Destination.hpp"

struct Level {
    std::vector<GravitySource> blackHoles;
    sf::Vector2f goalPos;

    Level(std::vector<GravitySource> holes, sf::Vector2f goal)
        : blackHoles(std::move(holes)), goalPos(goal) {}
};

class LevelManager {
public:
    int level = 0;
    std::vector<Level> levels;

    LevelManager();

    LevelManager(std::vector<Level> levels)
        : levels(std::move(levels)) {}

    void nextLevel();

    Level& currentLevel() {
        return levels[level];
    }
};

#endif // LEVEL_MANAGER_HPP