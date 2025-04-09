#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <optional>
#include <algorithm>

const float G = 6.67430e-11f;
const float MASS_INCREMENT = 1e15f;
const float MIN_MASS = 1e14f;
const float MAX_MASS = 1e16f;
const float SIZE_SCALE = 0.000000000000005f;
const float SPEED_INCREMENT = 50.f;

template <typename T>
T clamp(T value, T min, T max) {
    return std::max(min, std::min(value, max));
}

struct Planet {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    sf::CircleShape shape;

    Planet(float x, float y, float m, sf::Vector2f initialVelocity, sf::Color color)
        : position(x, y), velocity(initialVelocity), mass(m) {
        setMass(m);
    }

    void setMass(float newMass) {
        mass = clamp(newMass, MIN_MASS, MAX_MASS);
        float radius = std::max(5.f, mass * SIZE_SCALE);
        shape.setRadius(radius);
        shape.setFillColor(sf::Color::White);
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(position);
    }
};

sf::Vector2f computeGravity(const Planet& a, const Planet& b) {
    sf::Vector2f diff = b.position - a.position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (dist == 0) return sf::Vector2f(0, 0);
    float force = G * (a.mass * b.mass) / (dist * dist);
    sf::Vector2f dir = diff / dist;
    return dir * (force / a.mass);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1920u, 1080u}), "2D Planet Gravity Simulation with Sun and Boundaries");
    std::vector<Planet> planets;
    sf::Clock clock;

    float currentMass = 1e15f;
    sf::Vector2f initialVelocity(0.f, 0.f);

    // 屏幕中央的太阳
    sf::Vector2f center(window.getSize().x / 2.f, window.getSize().y / 2.f);
    Planet sun(center.x, center.y, 1e18f, sf::Vector2f(0.f, 0.f), sf::Color::Yellow);
    planets.push_back(sun);

    while (window.isOpen()) {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent) {
                    sf::Vector2f pos(static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y));
                    Planet newPlanet(pos.x, pos.y, currentMass, initialVelocity, sf::Color::White);
                    planets.push_back(newPlanet);
                }
            } else if (event->is<sf::Event::KeyPressed>()) {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    if (keyEvent->code == sf::Keyboard::Key::O) {
                        currentMass = clamp(currentMass + MASS_INCREMENT, MIN_MASS, MAX_MASS);
                    } else if (keyEvent->code == sf::Keyboard::Key::P) {
                        currentMass = clamp(currentMass - MASS_INCREMENT, MIN_MASS, MAX_MASS);
                    } else if (keyEvent->code == sf::Keyboard::Key::W) {
                        initialVelocity.y -= SPEED_INCREMENT;
                    } else if (keyEvent->code == sf::Keyboard::Key::S) {
                        initialVelocity.y += SPEED_INCREMENT;
                    } else if (keyEvent->code == sf::Keyboard::Key::A) {
                        initialVelocity.x -= SPEED_INCREMENT;
                    } else if (keyEvent->code == sf::Keyboard::Key::D) {
                        initialVelocity.x += SPEED_INCREMENT;
                    } else if (keyEvent->code == sf::Keyboard::Key::R) {
                        initialVelocity = sf::Vector2f(0.f, 0.f);
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        for (auto& p : planets) {
            if (&p == &sun) continue;  // 太阳不受引力影响，不更新

            // 计算引力加速度
            sf::Vector2f acc(0.f, 0.f);
            for (auto& other : planets) {
                if (&p != &other) acc += computeGravity(p, other);
            }
            p.velocity += acc * dt;

            // 更新位置
            p.position += p.velocity * dt;
            
            // 碰撞检测（不影响太阳）
            if (&p != &sun) {
                if (p.position.x - p.shape.getRadius() < 0) {
                    p.position.x = p.shape.getRadius();
                    p.velocity.x = -p.velocity.x;
                }
                if (p.position.x + p.shape.getRadius() > window.getSize().x) {
                    p.position.x = window.getSize().x - p.shape.getRadius();
                    p.velocity.x = -p.velocity.x;
                }
                if (p.position.y - p.shape.getRadius() < 0) {
                    p.position.y = p.shape.getRadius();
                    p.velocity.y = -p.velocity.y;
                }
                if (p.position.y + p.shape.getRadius() > window.getSize().y) {
                    p.position.y = window.getSize().y - p.shape.getRadius();
                    p.velocity.y = -p.velocity.y;
                }
            }

            p.shape.setPosition(p.position);
        }

        window.clear();
        for (auto& p : planets) window.draw(p.shape);
        window.display();
    }

    return 0;
}
