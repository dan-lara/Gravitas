/**
 * @file UIManager.cpp
 * @author Gravitas
 *
 * @brief Implementation of the UI Manager class and leaderboard rendering function.
 * @details
 * Used to the pages:
 * - Main Menu
 * - Leaderboard
 * - In Game
 */


#include "include/UIManager.hpp"

UIManager::UIManager():
    logoTexture("src/assets/imgs/gravitas.png"),
    logoSprite(logoTexture),
    titleText(font, "Gravitas - A Gravitational Adventure", 48),
    inputLabel(font, "Enter Player ID:", 24),
    playerInput(font, "|", 24),
    startButton(font, "Start Game", 30),
    leaderboardButton(font, "View Leaderboard", 30),
    levelText(font, "Level: 1", 24),
    scoreText(font, "Score: 0", 24),
    hintText(font, "Press R to restart\nPress Space to speed up\nPress B to use Lateral accelerator", 18)
        
{
    if (!font.openFromFile("src/assets/fonts/Arial.ttf")) 
        std::cerr << "Failed to load font\n";

    // Main Menu
    logoTexture.setSmooth(true);
    logoSprite.setTexture(logoTexture);
    logoSprite.setScale({0.5f, 0.5f});
    logoSprite.setPosition({10.f, 50.f});
        
    titleText.setPosition({600.f, 100.f});
    titleText.setFillColor(sf::Color::White);

    inputLabel.setPosition({600.f, 250.f});
    inputLabel.setFillColor(sf::Color::White);

    playerInput.setPosition({600.f, 290.f});
    playerInput.setFillColor(sf::Color::Cyan);

    startButton.setPosition({600.f, 350.f});
    leaderboardButton.setPosition({600.f, 400.f});

    // Jeu
    levelText.setPosition(sf::Vector2f(20.f, 20.f));
    scoreText.setPosition(sf::Vector2f(20.f, 50.f));
    hintText.setPosition(sf::Vector2f(20.f, 80.f));
}

void UIManager::handleEvent(const sf::Event& event, bool& startGame, bool& viewLeaderboard) {
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>();
        if (!key) return;

        if (key->code == sf::Keyboard::Key::Up) {
            selectedIndex = (selectedIndex - 1 + 2) % 2;
        } else if (key->code == sf::Keyboard::Key::Down) {
            selectedIndex = (selectedIndex + 1) % 2;
        } else if (key->code == sf::Keyboard::Key::Tab) {
            inputActive = !inputActive;
        } else if (key->code == sf::Keyboard::Key::Enter) {
            if (selectedIndex == 0 && !playerName.empty()) {
                startGame = true;
            } else if (selectedIndex == 1) {
                viewLeaderboard = true;
            }
        }
    }

    if (event.is<sf::Event::TextEntered>() && inputActive) {
        const auto* typed = event.getIf<sf::Event::TextEntered>();
        if (!typed) return;

        if (typed->unicode == '\b') {
            if (!playerName.empty()) playerName.pop_back();
        } else if (typed->unicode < 128 && playerName.size() < 15) {
            playerName += static_cast<char>(typed->unicode);
        }
        playerInput.setString(playerName + "|");
    }
}
void UIManager::render(sf::RenderWindow& window, int page) {
    if (page == 0) {
        renderMainMenu(window);
    } else if (page == 1) {
        renderLeaderboard(window);
    } else {
        renderGame(window);
    }
}

void UIManager::renderMainMenu(sf::RenderWindow& window){
    window.draw(logoSprite);
    startButton.setFillColor(selectedIndex == 0 ? sf::Color::Green : sf::Color(100, 100, 100));
    leaderboardButton.setFillColor(selectedIndex == 1 ? sf::Color::Yellow : sf::Color(100, 100, 100));
    
    window.draw(titleText);
    window.draw(inputLabel);
    window.draw(playerInput);
    window.draw(startButton);
    window.draw(leaderboardButton);
}

void UIManager::renderLeaderboard(sf::RenderWindow& window) {
    std::ifstream in("leaderboard.txt");
    if (!in.is_open()) {
        std::cerr << "fail to open\n";
        return;
    }

    std::vector<std::pair<std::string, float>> entries;
    std::string name;
    float time;

    while (in >> name >> time) {
        entries.emplace_back(name, time);
    }

    std::sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
        return a.second < b.second;
    });


    sf::Text title(font, "[ Leaderboard ]", 36);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(600.f, 80.f));

    window.draw(title);

    int maxDisplay = std::min(10, static_cast<int>(entries.size()));

    for (int i = 0; i < maxDisplay; ++i) {
        const auto& entry = entries[i];
        sf::Text line(font, std::to_string(i + 1) + ". " + entry.first + " - " + std::to_string(entry.second) + "s", 24);
        line.setFillColor(sf::Color::Cyan);
        line.setPosition(sf::Vector2f(600.f, 140.f + i * 30));
        window.draw(line);
    }

    sf::Text backHint(font, "Press P to return", 18);
    backHint.setFillColor(sf::Color(200, 200, 200));
    backHint.setPosition(sf::Vector2f(600.f, 500.f));
    window.draw(backHint);
}

void UIManager::renderGame(sf::RenderWindow& window) {
    // Render game-specific UI elements here
    // For example, level and score
    levelText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);
    hintText.setFillColor(sf::Color(200, 200, 200));

    window.draw(levelText);
    window.draw(scoreText);
    window.draw(hintText);
}

void UIManager::updateGameInfo(int level, int score) {
    levelText.setString("Level: " + std::to_string(level));
    scoreText.setString("Score: " + std::to_string(score));
}