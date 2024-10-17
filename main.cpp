#include "imgui.h"
#include "imgui-SFML.h"
#include "./header/Ball.h"
#include "SFML/Graphics.hpp"
#include "./header/PhysicsEngine.h"

#include <iostream>
#include<unordered_set>
using namespace std;


PhysicsEngine* PhysicsEngine::instancePtr = nullptr;

int main()
{
    int width = 900;
    int height = 900;
    sf::RenderWindow window(sf::VideoMode(width,height), "Window Title", sf::Style::Titlebar | sf::Style::Close);
    //ImGui::SFML::Init(window);
    
    std::unordered_set<Ball*>objects;
    for (int i = 0; i < 3; i++)
    {
        int ballposx = std::rand() % 800 + 30;
        int ballposy = std::rand() % 800 + 30;
        Ball* firstball = new Ball(ballposx, ballposy, (rand() % 30) + 30, 10);
        objects.insert(firstball);
    }

    int boxwidth = 800, boxheight = 800;
    sf::RectangleShape box(sf::Vector2f(boxwidth, boxheight));
    box.setOrigin(sf::Vector2f(boxwidth /2, boxheight /2));
    box.setPosition(sf::Vector2f(width / 2, height / 2));
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineThickness(10.f);
    box.setOutlineColor(sf::Color(250, 150, 100));

    //PhysicsEngine::getInstance()->gravity = sf::Vector2f(0.0, 500.0);
    PhysicsEngine::getInstance()->bh->setnew(boxwidth, boxheight, width / 2, height / 2);

    sf::Clock gameClock;


    while (window.isOpen())
    {
        //calling the process functions
        sf::Time deltatime = gameClock.getElapsedTime(); //deltatime
        double usec = deltatime.asMicroseconds();
        gameClock.restart(); //timer restart
        PhysicsEngine::getInstance()->process(usec / (1000000.000));


        for (auto& it : objects)
        {
            it->process(usec / (1000000.000));
        }


        //getting event
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                cout << "close button clicked " << endl;
                window.close();
            }

        }

        //draw function
        window.clear(sf::Color::Black);
        for (auto& it : objects)
        {
            it->draw(window);
        }

        //drawing bounding box;
        window.draw(box);


        window.display();
    }




}