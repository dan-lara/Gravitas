/**
 * @file UIManager.hpp
 * @author Gravitas
 *
 * @brief Definition of the UI Manager class and leaderboard rendering function.
 * @details
 * Used to the pages:
 * - Main Menu
 * - Leaderboard
 * - In Game
 */

#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class UIManager {
    // Generique
    sf::Font font;
    std::string playerName;

    // Menu principale
    sf::Texture logoTexture;
    sf::Text titleText;
    sf::Text inputLabel;
    sf::Text playerInput;
    sf::Text startButton;
    sf::Text leaderboardButton;
    sf::Sprite logoSprite;    
    
    // Dans le Jeu
    sf::Text levelText;
    sf::Text scoreText;
    sf::Text hintText;
    

    bool inputActive = false;
    int selectedIndex = 0; // 0: Start, 1: Leaderboard

    void renderMainMenu(sf::RenderWindow& window);
    void renderLeaderboard(sf::RenderWindow& window);
    void renderGame(sf::RenderWindow& window);

public:
    UIManager();

    void handleEvent(const sf::Event& event, bool& startGame, bool& viewLeaderboard);
    
    void render(sf::RenderWindow& window, int page);

    void updateGameInfo(int level, int score);

    std::string getPlayerName() const { return playerName; }

    sf::Font& getFont() { return font; }
};

#endif // UI_MANAGER_HPP
