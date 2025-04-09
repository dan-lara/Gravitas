#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <iostream>
#include <vector>

#include "../libs/include/GravitySource.hpp"
#include "../libs/include/Particle.hpp"

int main(){
    sf::RenderWindow window(sf::VideoMode({1600u, 1000u}), "Gravitas Simulation");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources;
    sources.push_back(GravitySource(sf::Vector2f(500.f, 500.f), 10.f, 2000.f, 0.f, sf::Color::White));
    sources.push_back(GravitySource(sf::Vector2f(1200.f, 500.f), 10.f, 2000.f, 0.f, sf::Color::White));

    int num_particles = 100;

    std::vector<Particle> particles;

    for (int i = 0; i < num_particles; i++)
    {
        particles.push_back(Particle(600, 700, 2, 0.2 + (0.1/num_particles) * i));

        particles.back().set_color(sf::Color(100 + (i * 155 / num_particles), 50, 200));
    }

    while (window.isOpen())
    {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent()))
        {
            if (event->is<sf::Event::Closed>()) 
                window.close();
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        window.clear();

        for(int i = 0;i < sources.size(); i++)
            for(int j = 0; j < particles.size(); j++)
                particles[j].updatePhysics(sources[i]);
        
        for(int i = 0;i < sources.size(); i++)
            sources[i].render(window);

        for(int j = 0;j < particles.size(); j++)
            particles[j].render(window);

        window.display();
    }

    return 0;
}
