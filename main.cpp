#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <optional>

class GravitySource {
    sf::Vector2f pos;
    float strength;
    sf::CircleShape shape;

public:
    GravitySource(float pos_x, float pos_y, float strength) {
        pos = {pos_x, pos_y};
        this->strength = strength;
        shape.setPosition(sf::Vector2f(pos.x, pos.y));
        shape.setFillColor(sf::Color::Black);
        shape.setRadius(20.f);
    }

    void render(sf::RenderWindow &window) { window.draw(shape); }
    sf::Vector2f getPos() { return pos; }
    float getStrength() { return strength; }

    void update(float dt) {
        shape.rotate(sf::degrees(50 * dt));  // 旋转 50°/秒
    }
    
    
};

class Spaceship {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape shape;
    std::vector<sf::Vector2f> trail; // 轨迹点
    bool active;

public:
    Spaceship(float pos_x, float pos_y)
        : pos(pos_x, pos_y), vel(0, 0), active(false) {
        shape.setPosition(pos);
        shape.setFillColor(sf::Color::White);
        shape.setRadius(8);
    }

    void setVelocity(sf::Vector2f velocity) {
        vel = velocity;
        active = true;
    }

    void updatePhysics(std::vector<GravitySource> &sources) {
        if (!active) return;
        trail.push_back(pos); // 记录轨迹

        for (auto &source : sources) {
            float dx = source.getPos().x - pos.x;
            float dy = source.getPos().y - pos.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < 50) {
                active = false;
                std::cout << "black hole!\n";
                return;
            }

            float inverseDist = 1.f / distance;
            float forceX = dx * inverseDist * inverseDist * source.getStrength();
            float forceY = dy * inverseDist * inverseDist * source.getStrength();

            vel.x += forceX;
            vel.y += forceY;
        }

        pos += vel;
    }

    void render(sf::RenderWindow &window) {
        // 画轨迹
        if (!trail.empty()) {
            std::vector<sf::Vertex> trailVertices;
            size_t trailSize = trail.size();
            for (size_t i = 0; i < trailSize; ++i) {
                // 颜色从深蓝到浅蓝
                float alpha = static_cast<float>(i) / trailSize * 255;
                trailVertices.push_back(sf::Vertex{sf::Vector2f(trail[i]), sf::Color(0, 255, 255, static_cast<unsigned char>(alpha))});
            }
            window.draw(trailVertices.data(), trailVertices.size(), sf::PrimitiveType::LineStrip);
        }


        shape.setPosition(pos);
        window.draw(shape);
    }

    void reset() {
        pos = {200, 800};
        vel = {0, 0};
        active = false;
        trail.clear(); // 清除轨迹
    }

    sf::Vector2f getPos() { return pos; }
    bool isActive() { return active; }
};

class Destination {
    sf::Vector2f pos;
    sf::CircleShape shape;

public:
    Destination(float x, float y) {
        pos = {x, y};
        shape.setPosition(sf::Vector2f(pos.x - 50, pos.y - 50));
        shape.setFillColor(sf::Color::Green);
        shape.setRadius(50.f);
    }

    void render(sf::RenderWindow &window) { window.draw(shape); }

    bool checkArrival(Spaceship &ship) {
        sf::Vector2f shipPos = ship.getPos();
        float dist = std::sqrt((pos.x - shipPos.x) * (pos.x - shipPos.x) +
                               (pos.y - shipPos.y) * (pos.y - shipPos.y));
        return dist < 50;
    }
};


class LevelManager {
    public:
        int level;   // 当前关卡
        int maxLevels;  // 最大关卡数
        std::vector<std::vector<GravitySource>> levels;  // 关卡黑洞数据
        std::vector<sf::Vector2f> goalPositions;  // 关卡目标点
    
        LevelManager() {
            level = 0;  // 初始关卡
            maxLevels = 3;  // 关卡数（可以扩展）
            
            // 定义关卡：黑洞位置 & 目标点
            levels = {
                {GravitySource(400, 400, 150)},  // 关卡 1
                {GravitySource(600, 500, 200), GravitySource(300, 300, 100)},  // 关卡 2
                {GravitySource(500, 500, 250), GravitySource(200, 200, 100), GravitySource(800, 400, 150)}  // 关卡 3
            };
    
            goalPositions = {
                {1400, 200},  // 关卡 1 目标
                {1200, 300},  // 关卡 2 目标
                {1000, 500}   // 关卡 3 目标
            };
        }
    
        void nextLevel() {
            if (level < maxLevels - 1) {
                level++;
                std::cout << "enter " << level + 1 << std::endl;
            } else {
                std::cout << "Salut Dani" << std::endl;
                level = 0;  // 重新开始
            }
        }
    };
    

// class UIManager {
//     sf::Font font;
//     sf::Text levelText;
//     sf::Text scoreText;
//     sf::Text hintText;

// public:
//     UIManager() {
//         if (!font.loadFromFile("arial.ttf")) {  // ⚠ 确保你的目录有字体文件
//             std::cerr << "无法加载字体文件！\n";
//         }

//         // 关卡文本
//         levelText.setFont(font);
//         levelText.setCharacterSize(24);
//         levelText.setFillColor(sf::Color::White);
//         levelText.setPosition(20, 20);

//         // 分数文本
//         scoreText.setFont(font);
//         scoreText.setCharacterSize(24);
//         scoreText.setFillColor(sf::Color::White);
//         scoreText.setPosition(20, 50);

//         // 提示文本
//         hintText.setFont(font);
//         hintText.setCharacterSize(18);
//         hintText.setFillColor(sf::Color(200, 200, 200));
//         hintText.setPosition(20, 80);
//         hintText.setString("鼠标拖动发射飞船\n按 R 重新开始");
//     }

//     void update(int level, int score) {
//         levelText.setString("关卡: " + std::to_string(level + 1));
//         scoreText.setString("分数: " + std::to_string(score));
//     }

//     void render(sf::RenderWindow &window) {
//         window.draw(levelText);
//         window.draw(scoreText);
//         window.draw(hintText);
//     }
// };







int main() {
    sf::RenderWindow window(sf::VideoMode({1600u, 1000u}), "Level System");
    window.setFramerateLimit(60);

    // std::vector<GravitySource> blackHoles = {
    //     //GravitySource(800, 500, 200),
    //     GravitySource(400, 400, 150)
    // };

    LevelManager levelManager;  // 添加关卡管理
    std::vector<GravitySource> blackHoles = levelManager.levels[levelManager.level];  // 加载当前关卡黑洞

    sf::Clock clock;

    int score = 0;




    Spaceship ship(200, 800);
    //Destination goal(1400, 200);
    Destination goal(levelManager.goalPositions[levelManager.level].x, levelManager.goalPositions[levelManager.level].y);

    bool gameStarted = false;
    bool dragging = false;
    sf::Vector2f launchStart, launchVelocity;

    while (window.isOpen()) {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();

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
                        gameStarted = true;
                    }
                    dragging = false;
                }
            }

            if (event->is<sf::Event::KeyPressed>()) {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    if (keyEvent->code == sf::Keyboard::Key::R) {
                        // 重置游戏状态
                        ship.reset(); // 重新创建飞船
                        blackHoles = levelManager.levels[levelManager.level];
                        goal = Destination(levelManager.goalPositions[levelManager.level].x, levelManager.goalPositions[levelManager.level].y);
                        gameStarted = false;        // 允许重新拖拽
                        dragging = false;
                        std::cout << "Game Reset!\n";
                    }
                }
            }
            
        }

        ship.updatePhysics(blackHoles);


        if (goal.checkArrival(ship)) {
            score += 10;
            std::cout << "win!\n";
            std::cout << "score: " << score << std::endl;

            if(score == 30) {
                score = 0;
                std::cout << "next level\n";
                levelManager.nextLevel();
                blackHoles = levelManager.levels[levelManager.level];
                goal = Destination(levelManager.goalPositions[levelManager.level].x, levelManager.goalPositions[levelManager.level].y);
                ship.reset();
                gameStarted = false;
            }

            gameStarted = false;
        }
        

        float deltaTime = clock.restart().asSeconds();
        for (auto &bh : blackHoles) bh.update(deltaTime);

        window.clear(sf::Color(30, 30, 30));
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
        }
        

        window.display();
    }

    return 0;
}
