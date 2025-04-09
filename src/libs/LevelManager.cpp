/**
 * @file LevelManager.hpp
 * @author Gravitas
 *
 * @brief Implementation of the LevelManager class.
 * @details
 * The LevelManager class is responsible for managing the levels in the game.
 * It creates the game turney and handles the transition between levels.
 */

#include "include/LevelManager.hpp"

LevelManager::LevelManager() {
    levels.emplace_back(
        std::vector<GravitySource>{
            GravitySource({400, 400}, 20.f, 150.f, 0.f)
        },
        sf::Vector2f(1400, 200)
    );
    
    levels.emplace_back(
        std::vector<GravitySource>{
            GravitySource({600, 500}, 20.f, 180.f, 45.f)
        },
        sf::Vector2f(1450, 100)
    );
    
    levels.emplace_back(
        std::vector<GravitySource>{
            GravitySource({400, 400}, 20.f, 100.f, 60.f),
            GravitySource({800, 400}, 20.f, 100.f, -60.f)
        },
        sf::Vector2f(1350, 100)
    );
    
    levels.emplace_back(
        std::vector<GravitySource>{
            GravitySource({400, 600}, 20.f, 150.f, 0.f),
            GravitySource({600, 500}, 25.f, 180.f, -45.f)
        },
        sf::Vector2f(1450, 100)
    );
}

void LevelManager::nextLevel() {
    level = (level + 1) % levels.size();
}
