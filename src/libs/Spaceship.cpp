/**
 * @file Spaceship.cpp
 * @author Gravitas
 *
 * @brief Implementation of the Spaceship class.
 */

#include "include/Spaceship.hpp"
#include "include/Logger.hpp"

Spaceship::Spaceship(float pos_x, float pos_y)
    : pos(pos_x, pos_y), vel(0, 0), active(false), pos0(pos),
      shape(8.f), spaceshipTexture(SHIP_PATH), spaceshipTextureCrash(SHIP_CRASH_PATH), sprite(spaceshipTexture) 
    {
        if (!spaceshipTexture.loadFromFile(SHIP_PATH)) {
            std::cerr << "Failed to load spaceship texture\n";
            Logger::log("Failed to load spaceship texture", Logger::Level::ERROR,true,true);
        }
        if (!spaceshipTextureCrash.loadFromFile(SHIP_CRASH_PATH)) {
            std::cerr << "Failed to load spaceship crash texture\n";
            Logger::log("Failed to load spaceship crash texture", Logger::Level::ERROR,true,true);
        }
        sprite.setTexture(spaceshipTexture);
        sprite.setOrigin({float(spaceshipTexture.getSize().x) / 2, float(spaceshipTexture.getSize().y) / 2});
        sprite.setPosition(pos);
        sprite.setScale({SHIP_SCALE, SHIP_SCALE});
        spriteLoaded = true;
    }

Spaceship::Spaceship(float pos_x, float pos_y, float size)
    : pos(pos_x, pos_y), vel(0, 0), active(false), pos0(pos),
    shape(size), spaceshipTexture(SHIP_PATH), spaceshipTextureCrash(SHIP_CRASH_PATH), sprite(spaceshipTexture) 
    {
        shape.setOrigin(pos);
        shape.setPosition(pos);
        shape.setFillColor(sf::Color::White);
        if(!size)
            size = SHIP_RADIUS;
        shape.setRadius(size);
    }

Spaceship::Spaceship(float pos_x, float pos_y, std::string texturePath, float scale)
    : pos(pos_x, pos_y), vel(0, 0), active(false), pos0(pos),
      shape(8.f), spaceshipTexture(texturePath), spaceshipTextureCrash(texturePath), sprite(spaceshipTexture) 
    {
        if (!spaceshipTexture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load spaceship texture\n";
            Logger::log("Failed to load spaceship texture", Logger::Level::ERROR,true,true);
        }
        sprite.setTexture(spaceshipTexture);
        sprite.setOrigin({float(spaceshipTexture.getSize().x) / 2, float(spaceshipTexture.getSize().y) / 2});
        sprite.setPosition(pos);
        sprite.setScale({scale, scale});
        spriteLoaded = true;
    }

void Spaceship::setVelocity(sf::Vector2f velocity) {
    vel = velocity;
    active = true;
}

void Spaceship::reset() {
    pos = pos0;
    vel = {0.f, 0.f};
    active = false;
    hasFailed = false;
    boosting = false;
    fuelParticles.clear();
    trail.clear();
}

void Spaceship::updateParticles(float dt) {
    for (auto& p : fuelParticles) {
        p.pos += p.vel;
        p.lifetime -= dt;
        p.color.a = static_cast<uint8_t>(255 * (p.lifetime / 0.5f));
    }
    // Supprimer les particules dont la durée de vie est terminée
    fuelParticles.erase(std::remove_if(fuelParticles.begin(), fuelParticles.end(),
        [](const FuelParticle &p) { return p.lifetime <= 0.f; }), fuelParticles.end());
}

void Spaceship::updatePhysics(std::vector<GravitySource> &sources, float dt) {
    if (!active) return;

    trail.push_back(pos); // Enregistrer la trajectoire

    trail.push_back(pos);
    updateParticles(dt);  // Mettre à jour les particules

    for (auto &source : sources) {
        float dx = source.getPos().x - pos.x;
        float dy = source.getPos().y - pos.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < source.getRadius()) {
            active = false;
            hasFailed = true;
            std::cout << "Black Hole!\n";
            Logger::log("Black Hole!", Logger::Level::INFO,true,true);
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

void Spaceship::render(sf::RenderWindow &window) {

    // Dessiner les particules
    for (const auto& p : fuelParticles) {
        sf::CircleShape fire(3);
        fire.setPosition(p.pos);
        fire.setFillColor(p.color);
        window.draw(fire);
    }


    // Dessiner la trajectoire
    if (!trail.empty()) {
        std::vector<sf::Vertex> trailVertices;
        size_t trailSize = trail.size();
        for (size_t i = 0; i < trailSize; ++i) {
            // Couleur du bleu foncé au bleu clair
            float alpha = static_cast<float>(i) / trailSize * 255;
            trailVertices.push_back(sf::Vertex{sf::Vector2f(trail[i]), sf::Color(0, 255, 255, static_cast<unsigned char>(alpha))});
        }
        window.draw(trailVertices.data(), trailVertices.size(), sf::PrimitiveType::LineStrip);
    }

    if(!spriteLoaded) {
        shape.setPosition(pos);
        window.draw(shape);
    } else  {
        if (hasFailed) {
            sprite.setTexture(spaceshipTextureCrash);
            sprite.setOrigin({float(spaceshipTextureCrash.getSize().x) / 2, float(spaceshipTextureCrash.getSize().y) / 2});
        } else {
            sprite.setTexture(spaceshipTexture);
            sprite.setOrigin({float(spaceshipTexture.getSize().x) / 2, float(spaceshipTexture.getSize().y) / 2});
        }
        
        sprite.setScale({SHIP_SCALE, SHIP_SCALE});
        sprite.setPosition(pos);

        sprite.setRotation(sf::degrees(std::atan2(vel.y, vel.x) * 180.f / PI + 90.f));
        window.draw(sprite);
    }
}


std::vector<sf::Vector2f> Spaceship::predictTrajectory(
    const std::vector<GravitySource>& sources,
    sf::Vector2f initialVelocity,
    int steps,
    float dt
) const {
    std::vector<sf::Vector2f> prediction;
    sf::Vector2f futurePos = pos;
    sf::Vector2f futureVel = initialVelocity;

    for (int i = 0; i < steps; ++i) {
        prediction.push_back(futurePos);

        // Calculer la force gravitationnelle
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
    }

    return prediction;
}


void Spaceship::boost1(){
    if (active) {
        vel *= 1.5f;  // Accélération simple (peut être modifiée pour une direction opposée)
        boosting = true;

        // Générer des particules
        for (int i = 0; i < 10; ++i) {
            sf::Vector2f randomOffset{(rand() % 10 - 5) * 0.2f, (rand() % 10 - 5) * 0.2f};
            sf::Vector2f particleVel = -vel * 0.2f + randomOffset;
            fuelParticles.emplace_back(pos, particleVel);
        }
    }
}

void Spaceship::boost2(){
    if (active) {
        vel += {-vel.y * 0.1f, vel.x * 0.1f};
        boosting = true;

        // Générer des particules
        for (int i = 0; i < 10; ++i) {
            sf::Vector2f randomOffset{(rand() % 10 - 5) * 0.2f, (rand() % 10 - 5) * 0.2f};
            sf::Vector2f particleVel = -vel * 0.2f + randomOffset;
            fuelParticles.emplace_back(pos, particleVel);
        }
    }
}