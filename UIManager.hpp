/**
 * @file UIManager.hpp
 * @author Gravitas
 *
 * @brief Description
 */

#pragma once

class UIManager {
    sf::Font font;
    sf::Text levelText;
    sf::Text scoreText;
    sf::Text hintText;

public:
    UIManager() :
        levelText(font, "level: 1", 24),
        scoreText(font, "score: 0", 24),
        hintText(font, "mouse drag for starting \npress R to restart \npress Space to speed up \npress B to ues Lateral accelerator", 18)
    {
        if (!font.openFromFile("src/assets/fonts/Arial.ttf")) {
            std::cerr << "can not load arial.ttf\n";
        }

        levelText.setPosition(sf::Vector2f(20.f, 20.f));
        scoreText.setPosition(sf::Vector2f(20.f, 50.f));
        hintText.setPosition(sf::Vector2f(20.f, 80.f));

        levelText.setFillColor(sf::Color::White);
        scoreText.setFillColor(sf::Color::White);
        hintText.setFillColor(sf::Color(200, 200, 200));
    }

    void update(int level, int score) {
        levelText.setString("level: " + std::to_string(level + 1));
        scoreText.setString("score: " + std::to_string(score));
    }

    void render(sf::RenderWindow& window) {
        window.draw(levelText);
        window.draw(scoreText);
        window.draw(hintText);
    }
};