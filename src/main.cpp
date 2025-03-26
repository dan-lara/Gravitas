#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cmath>
#include <iostream>
#include <vector>
#include <optional>
#include <cstdint>



struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float lifetime;
    sf::Color color;

    Particle(sf::Vector2f p, sf::Vector2f v)
        : pos(p), vel(v), lifetime(0.5f), color(sf::Color::Red) {}
};



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
    sf::Vector2f getPos() const { return pos; }
    float getStrength() const { return strength; }

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
    bool hasFailed = false;
    std::vector<Particle> particles;//粒子
    bool boosting = false;



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

    void updatePhysics(std::vector<GravitySource> &sources, float dt) {
        if (!active) return;
        trail.push_back(pos); // 记录轨迹

        trail.push_back(pos);
        updateParticles(dt);  // 更新粒子

        for (auto &source : sources) {
            float dx = source.getPos().x - pos.x;
            float dy = source.getPos().y - pos.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < 50) {
                active = false;
                hasFailed = true;
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

        // 画粒子
        for (const auto& p : particles) {
            sf::CircleShape fire(3);
            fire.setPosition(p.pos);
            fire.setFillColor(p.color);
            window.draw(fire);
        }


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

    //预测轨迹
    std::vector<sf::Vector2f> predictTrajectory(
        const std::vector<GravitySource>& sources,
        sf::Vector2f initialVelocity,
        int steps = 200,
        float dt = 1.0f
    ) const {
        std::vector<sf::Vector2f> prediction;
    
        sf::Vector2f futurePos = pos;
        sf::Vector2f futureVel = initialVelocity;
    
        for (int i = 0; i < steps; ++i) {
            sf::Vector2f acc(0.f, 0.f);
            for (const auto& source : sources) {
                sf::Vector2f diff = source.getPos() - futurePos;
                float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (dist < 1.f) continue;
    
                float force = source.getStrength() / (dist * dist);
                sf::Vector2f dir = diff / dist;
                acc += dir * force;
            }
    
            futureVel += acc * dt;
            futurePos += futureVel * dt;
            prediction.push_back(futurePos);
        }
    
        return prediction;
    }
    

    //正向加速
    void boost1() {
        if (active) {
            vel *= 1.5f;  // 简单加速方向（可改为背向方向）
            boosting = true;
    
            // 生成粒子
            for (int i = 0; i < 10; ++i) {
                sf::Vector2f randomOffset{(rand() % 10 - 5) * 0.2f, (rand() % 10 - 5) * 0.2f};
                sf::Vector2f particleVel = -vel * 0.2f + randomOffset;
                particles.emplace_back(pos, particleVel);
            }
        }
    }

    //侧向加速
    void boost2() {
        if (active) {
            vel += {-vel.y * 0.1f, vel.x * 0.1f};
            boosting = true;
    
            // 生成粒子
            for (int i = 0; i < 10; ++i) {
                sf::Vector2f randomOffset{(rand() % 10 - 5) * 0.2f, (rand() % 10 - 5) * 0.2f};
                sf::Vector2f particleVel = -vel * 0.2f + randomOffset;
                particles.emplace_back(pos, particleVel);
            }
        }
    }
    

    //渲染粒子
    void updateParticles(float dt) {
        for (auto& p : particles) {
            p.pos += p.vel;
            p.lifetime -= dt;
            p.color.a = static_cast<uint8_t>(255 * (p.lifetime / 0.5f));
        }
    
        // 删除寿命结束的粒子
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.lifetime <= 0.f; }), particles.end());
    }



    void reset() {
        pos = {200, 800};
        vel = {0, 0};
        active = false;
        hasFailed = false;
        trail.clear(); // 清除轨迹
    }

    sf::Vector2f getPos() { return pos; }
    bool isActive() { return active; }
    bool getFailureStatus() const { return hasFailed; }
    void clearFailureStatus() { hasFailed = false; }

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
    

class UIManager {
    sf::Font font;
    sf::Text levelText;
    sf::Text scoreText;
    sf::Text hintText;

public:
    UIManager() :
        // 正确构造 text（传入字体、字符串、字号）
        levelText(font, "level: 1", 24),
        scoreText(font, "score: 0", 24),
        hintText(font, "mouse drag for starting \npress R to restart \npress Space to speed up \npress B to ues Lateral accelerator", 18)
    {
        if (!font.openFromFile("arial.ttf")) {
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
    

class SoundManager {
    sf::SoundBuffer launchBuffer, winBuffer, failBuffer;
    sf::Sound launchSound;
    sf::Sound winSound;
    sf::Sound failSound;

public:
    SoundManager()
        : launchSound(launchBuffer),
          winSound(winBuffer),
          failSound(failBuffer)
    {
        if (!launchBuffer.loadFromFile("launch.wav")) std::cerr << "Failed to load launch.wav\n";
        if (!winBuffer.loadFromFile("win.wav")) std::cerr << "Failed to load win.wav\n";
        if (!failBuffer.loadFromFile("fail.wav")) std::cerr << "Failed to load fail.wav\n";
    }

    void playLaunch() { launchSound.play(); }
    void playWin()    { winSound.play(); }
    void playFail()   { failSound.play(); }
};









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

    SoundManager sound;



    Spaceship ship(200, 800);
    //Destination goal(1400, 200);
    Destination goal(levelManager.goalPositions[levelManager.level].x, levelManager.goalPositions[levelManager.level].y);
    UIManager ui;  // ✅ 添加 UI 管理器

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
                        sound.playLaunch();
                        gameStarted = true;
                    }
                    dragging = false;
                }
            }


            //按R重置
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
            

            //boost1
            if (event->is<sf::Event::KeyPressed>()) {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    if (keyEvent->code == sf::Keyboard::Key::Space) {
                        ship.boost1();
                    }
                }
            }

            //boost2
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

        ship.updatePhysics(blackHoles,deltaTime);

        if (ship.getFailureStatus()) {
            sound.playFail();
            ship.clearFailureStatus();
        }
        
        


        if (goal.checkArrival(ship)) {
            score += 10;
            std::cout << "win!\n";
            sound.playWin();
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
        


        for (auto &bh : blackHoles) bh.update(deltaTime);

        window.clear(sf::Color(30, 30, 30));
        for (auto &bh : blackHoles) bh.render(window);
        goal.render(window);
        ship.render(window);

        if (dragging) {
            //
            sf::Vertex line[] = {
                sf::Vertex{sf::Vector2f(launchStart.x, launchStart.y), sf::Color::White},
                sf::Vertex{sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x), 
                                        static_cast<float>(sf::Mouse::getPosition(window).y)), 
                           sf::Color::White}
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);

            //
            sf::Vector2f currentMouse = sf::Vector2f(sf::Mouse::getPosition(window));

            sf::Vector2f simulatedVelocity = (launchStart - currentMouse) * 0.1f;
            auto prediction = ship.predictTrajectory(blackHoles, simulatedVelocity);

            std::vector<sf::Vertex> preview;
            for (const auto& p : prediction) {
                sf::Vertex v;
                v.position = p;
                v.color = sf::Color(255, 255, 0, 80); // 半透明黄色
                preview.push_back(v);
            }
            window.draw(preview.data(), preview.size(), sf::PrimitiveType::LineStrip);
            

            

        }
        
        ui.update(levelManager.level, score);  // ✅ 更新 UI
        ui.render(window);  // ✅ 渲染 UI



        window.display();
    }

    return 0;
}
