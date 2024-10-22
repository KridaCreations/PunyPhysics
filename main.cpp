#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "./header/PhysicsWorld.h"
#include "./header/PhysicsEngine.h"
#include <iostream>
#include<unordered_set>
using namespace std;

PhysicsWorld* PhysicsWorld::instancePtr = nullptr;
PhysicsEngine* PhysicsEngine::instancePtr = nullptr;
int main()
{
    int width = 900;
    int height = 900;
    sf::RenderWindow window(sf::VideoMode(width,height), "Window Title", sf::Style::Titlebar | sf::Style::Close);
    

    sf::Clock gameClock;

  
    while (window.isOpen())
    {
       
        //calling the process functions
        sf::Time deltatime = gameClock.getElapsedTime(); //deltatime
        double usec = deltatime.asMicroseconds();
        gameClock.restart(); //timer restart
        PhysicsWorld::getInstance()->process(usec / (1000000.000));


     
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
       

        window.display();
    }




}