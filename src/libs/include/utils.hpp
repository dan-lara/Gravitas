/**
 * @file constants.hpp
 * @author Gravitas
 *
 * @brief the physics constants used in the game.
 */

#ifndef UTILS_HPP
#define UTILS_HPP
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

constexpr double G = 6.67430e-11; // Gravitational constant

constexpr double PI = 3.14159265358979323846; // Pi constant

inline constexpr double toRadians(double degrees) {
    return degrees * PI / 180.0;
}

inline constexpr double toDegrees(double radians) {
    return radians * 180.0 / PI;
}

inline constexpr double distance(sf::Vector2f a, sf::Vector2f b) {
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

inline constexpr double distance(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

#endif // UTILS_HPP