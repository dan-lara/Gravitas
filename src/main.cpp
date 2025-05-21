#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cmath>
#include <iostream>
#include <fstream>

#include <vector>
#include <optional>
#include <cstdint>

#include "libs/include/GravitySource.hpp"
#include "libs/include/Spaceship.hpp"
#include "libs/include/Destination.hpp"
#include "libs/include/LevelManager.hpp"
#include "libs/include/UIManager.hpp"
#include "libs/include/SoundManager.hpp"
#include "libs/include/utils.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({1600u, 1000u}), "Gravitas");
    window.setFramerateLimit(60);

    UIManager menu;
    bool inMenu = true;
    bool inGame = false;
    bool viewingLeaderboard = false;

    std::string currentPlayerID;
    sf::Clock gameClock;
    float elapsedTime = 0.f;

    LevelManager levelManager;  // Ajouter gestion des niveaux
    Level& current = levelManager.currentLevel();
    std::vector<GravitySource>& blackHoles = current.blackHoles;

    sf::Clock clock;
    int score = 0;
    SoundManager sound;

    Spaceship ship(200, 800);
    Destination goal(current.goalPos.x, current.goalPos.y);

    bool gameStarted = false;
    bool dragging = false;
    sf::Vector2f launchStart, launchVelocity;

    while (window.isOpen()) {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();

            // menu
            if (inMenu) {
                menu.handleEvent(*event, inGame, viewingLeaderboard);
                if (inGame) {
                    inMenu = false;
                    currentPlayerID = menu.getPlayerName();
                    gameClock.restart(); // Commencer le chronomètre
                } else if (viewingLeaderboard) {
                    inMenu = false;  // ✅ Entrer dans le classement quitte également le menu
                }
                continue; // État du menu principal, ne pas continuer la logique ci-dessous
            }

            // lancer
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
                    if (!gameStarted) {
                        dragging = true;
                        launchStart = sf::Vector2f(mouseEvent->position.x, mouseEvent->position.y);
                    }
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>()) {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>();
                if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
                    if (dragging && !gameStarted) {
                        sf::Vector2f launchEnd(mouseEvent->position.x, mouseEvent->position.y);
                        launchVelocity = (launchStart - launchEnd) * 0.1f;
                        ship.setVelocity(launchVelocity);
                        sound.playLaunch();
                        gameStarted = true;
                    }
                    dragging = false;
                }
            }

            // appuyer sur R pour réinitialiser
            if (event->is<sf::Event::KeyPressed>()) {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    if (keyEvent->code == sf::Keyboard::Key::R) {
                        ship.reset(); // Réinitialiser le vaisseau
                        Level& current = levelManager.currentLevel();
                        blackHoles = current.blackHoles;
                        goal = Destination(current.goalPos.x, current.goalPos.y);
                        gameStarted = false; // Permettre un nouveau lancement
                        dragging = false;
                        std::cout << "Game Reset!\n";
                    }
                }
            }

            // boost1
            if (event->is<sf::Event::KeyPressed>()) {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    if (keyEvent->code == sf::Keyboard::Key::Space) {
                        ship.boost1();
                    }
                }
            }

            // boost2
            if (event->is<sf::Event::KeyPressed>()) {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    if (keyEvent->code == sf::Keyboard::Key::B) {
                        ship.boost2();
                    }
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        ship.updatePhysics(blackHoles, deltaTime);

        if (ship.getFailureStatus()) {
            ship.render(window);
            sound.playFail();
            ship.clearFailureStatus();
        }

        if (goal.checkArrival(ship)) {
            score += 10;
            std::cout << "win!\n";
            sound.playWin();
            std::cout << "score: " << score << std::endl;

            if (score == 30) {
                score = 0;
                std::cout << "next level\n";
                levelManager.nextLevel();
                Level& current = levelManager.currentLevel();
                blackHoles = current.blackHoles;
                goal = Destination(current.goalPos.x, current.goalPos.y);
                ship.reset();
                gameStarted = false;
            }
            gameStarted = false;
        }

        for (auto &bh : blackHoles) bh.update(deltaTime);

        // window.clear(sf::Color(169, 169, 169)); // Medium gray color
        window.clear(sf::Color(0, 0, 0)); // Clear the window with black color

        if (inMenu) {
            menu.render(window, 0); // Menu principal
        } else if (inGame) {
            if (goal.checkArrival(ship)) {
                score += 10;
                std::cout << "win!\n";
                sound.playWin();
                std::cout << "score: " << score << std::endl;

                if (levelManager.level == 4) {
                    elapsedTime = gameClock.getElapsedTime().asSeconds();
                    std::cout << "Player " << currentPlayerID << " time: " << elapsedTime << "s\n";
                    std::ofstream out("leaderboard.txt", std::ios::app);
                    out << currentPlayerID << " " << elapsedTime << "\n";
                    out.close();
                    inGame = false;
                    inMenu = true;
                    ship.reset();
                    score = 0;
                    levelManager.level = 0;
                    blackHoles = levelManager.currentLevel().blackHoles;
                    goal = Destination(levelManager.currentLevel().goalPos.x, levelManager.currentLevel().goalPos.y);
                    continue;
                } else {
                    levelManager.nextLevel();
                    Level& current = levelManager.currentLevel();
                    blackHoles = current.blackHoles;
                    goal = Destination(current.goalPos.x, current.goalPos.y);
                    ship.reset();
                    gameStarted = false;
                }
            }

            for (auto &bh : blackHoles) bh.render(window);
            goal.render(window);
            ship.render(window);

            if (dragging) {
                sf::Vertex line[] = {
                    sf::Vertex{sf::Vector2f(launchStart.x, launchStart.y), sf::Color::White},
                    sf::Vertex{sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x), 
                                            static_cast<float>(sf::Mouse::getPosition(window).y)), 
                               sf::Color::White}
                };
                window.draw(line, 2, sf::PrimitiveType::Lines);

                sf::Vector2f currentMouse = sf::Vector2f(sf::Mouse::getPosition(window));
                sf::Vector2f simulatedVelocity = (launchStart - currentMouse) * 0.1f;
                auto prediction = ship.predictTrajectory(blackHoles, simulatedVelocity);

                std::vector<sf::Vertex> preview;
                for (const auto& p : prediction) {
                    sf::Vertex v;
                    v.position = p;
                    v.color = sf::Color(255, 255, 0, 80); // Jaune semi-transparent
                    preview.push_back(v);
                }
                window.draw(preview.data(), preview.size(), sf::PrimitiveType::LineStrip);
            }

            menu.updateGameInfo(levelManager.level, score); // Mettre à jour les infos du jeu
            menu.render(window, 2); // Rendre les infos du jeu
        } else if (viewingLeaderboard) {
            menu.render(window, 1); // Afficher le classement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
                viewingLeaderboard = false;
                inMenu = true;
            }
        }

        window.display();
    }

    return 0;
}
