#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <iostream>
#include <vector>

class GravitySource {

    sf::Vector2f pos;
    float strength;
    sf::CircleShape shape;

public:
    GravitySource(float pos_x, float pos_y, float strength){
       pos.x = pos_x;
       pos.y = pos_y;
       this->strength = strength;

       shape.setPosition(pos);
       shape.setFillColor(sf::Color::White);
       shape.setRadius(10.f);

    

    }


    void render(sf::RenderWindow &window){
        window.draw(shape); 
    }

    sf::Vector2f get_Pos(){
        return pos;
    }

    float get_Strength(){
        return strength;
    }
};




class Particle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape shape;
 
 
public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y)
    {
        pos.x = pos_x;
        pos.y = pos_y;
 
        vel.x = vel_x;
        vel.y = vel_y;
 
        shape.setPosition(pos);
        shape.setFillColor(sf::Color::White);
        shape.setRadius(5);
    }

    void set_color(sf::Color color)
    {
        shape.setFillColor(color);
    }
 
    void render(sf::RenderWindow& wind)
    {
        shape.setPosition(pos);
        wind.draw(shape);
    }
    
    void update_physics(GravitySource &source)
    {
        float distance_x = source.get_Pos().x - pos.x;  //4
        float distance_y = source.get_Pos().y - pos.y;  //3

        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);  //5

        float inverse_distance = 1.f / distance;    // 1/5

        float normalized_x = distance_x * inverse_distance; //单位方向向量
        float normalized_y = distance_y * inverse_distance;

        float acceleration_x = normalized_x * source.get_Strength() * inverse_distance * inverse_distance; //方向向量 * 强度 * 1/距离的平方 
        float acceleration_y = normalized_y * source.get_Strength() * inverse_distance * inverse_distance;

        vel.x += acceleration_x;
        vel.y += acceleration_y;

        pos.x += vel.x;
        pos.y += vel.y;



 
    }
 
};




int main()
{
    sf::RenderWindow window(sf::VideoMode({1600u, 1000u}), "My Program");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources;
    sources.push_back(GravitySource(500, 500, 2000));
    sources.push_back(GravitySource(1200, 500, 2000));

    int num_particles = 100;

    std::vector<Particle> particles;

    for (int i = 0; i < num_particles; i++)
    {
        particles.push_back(Particle(600, 700, 2, 0.2 + (0.1/num_particles) * i));

        particles.back().set_color(sf::Color(100 + (i * 155 / num_particles), 50, 200));
    }


    //GravitySource source(800, 500, 2000);
    //Particle particle(600, 500, 0, 3); 

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
        // draw calls
        // particle.update_physics(source);

        // source.render(window);
        // particle.render(window);

        for(int i = 0;i < sources.size(); i++)
        {
            for(int j = 0; j < particles.size(); j++)
            {
                particles[j].update_physics(sources[i]);
            }
        }
        
        for(int i = 0;i < sources.size(); i++){

            sources[i].render(window);

        }

        for(int j = 0;j < particles.size(); j++){

            particles[j].render(window);
            
        }







        window.display();
    }

    return 0;
}
