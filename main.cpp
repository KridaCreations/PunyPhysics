#include "imgui.h"
#include "imgui-SFML.h"
#include "./header/Ball.h"
#include "./header/staticball.h"
#include "SFML/Graphics.hpp"
#include "./header/PhysicsEngine.h"
#include "./header/rod.h"
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
    std::unordered_set<staticball*>hardobjects;
    std::vector<sf::Color>colors = {
        sf::Color::Cyan,
        sf::Color::Green,
        sf::Color::Red,
        sf::Color::Magenta,
        sf::Color::Blue,
        sf::Color::Yellow
    };
    for (int i = 0; i <200 ;i++)
    {
        int ballposx = std::rand() % 800 + 30;
        int ballposy = std::rand() % 800 + 30;
        int mass = (rand() % 50) + 20;
        Ball* firstball = new Ball(ballposx,ballposy,20, 50, colors[rand() % colors.size()]);
        objects.insert(firstball);
    }

    Rod* spinner = new Rod(450,450,15,10);

    int boxwidth = 800, boxheight = 800;
    sf::RectangleShape box(sf::Vector2f(boxwidth, boxheight));
    box.setOrigin(sf::Vector2f(boxwidth /2, boxheight /2));
    box.setPosition(sf::Vector2f(width / 2, height / 2));
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineThickness(10.f);
    box.setOutlineColor(sf::Color(250, 150, 100));

    PhysicsEngine::getInstance()->coeffofrestitution = 1;
    //PhysicsEngine::getInstance()->gravity = sf::Vector2f(0,1000);
    PhysicsEngine::getInstance()->bh->setnew(boxwidth, boxheight, width / 2, height / 2);

    sf::Clock gameClock;

    int counter = 50;
    vector<pair<int, int>>dir = {
        {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}
        //{1,0},{1,1},{0,1},{-1,1},{-1,0}
        //{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,1},{0,1},{1,1}
    };
    int dirindex = 0;
    while (window.isOpen())
    {
        counter--;
        //std::cout << counter << std::endl;
        if (counter < 0)
        {
            counter = 50;
            //std::cout <<"gdir " << dir[dirindex].first << " " << dir[dirindex].second << std::endl;
            //PhysicsEngine::getInstance()->gravity = sf::Vector2f(dir[dirindex].first * 500, dir[dirindex].second * 500);
            dirindex++;
            dirindex = dirindex % dir.size();
        }
        //calling the process functions
        sf::Time deltatime = gameClock.getElapsedTime(); //deltatime
        double usec = deltatime.asMicroseconds();
        gameClock.restart(); //timer restart
        PhysicsEngine::getInstance()->process(usec / (1000000.000));


        

        spinner->process(usec / 1000000.000);
        for (auto& it : objects)
        {
            it->process(usec / (1000000.000));
        }
        for (auto& it : hardobjects)
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
        spinner->draw(window);
        for (auto& it : objects)
        {
            it->draw(window);
        }
        for (auto& it : hardobjects)
        {
            it->draw(window);
        }

        //drawing bounding box;
        window.draw(box);


        window.display();
    }




}