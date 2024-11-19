#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "./header/PhysicsWorld.h"
#include "./header/Circle.h"
#include "./header/Rectangle.h"

#include <iostream>
#include<unordered_set>
#include "header/Polygon.h"
using namespace std;

PhysicsWorld* PhysicsWorld::instancePtr = nullptr;

//sf::RenderWindow* win;

int main()
{
    int width = 900;
    int height = 900;
    sf::RenderWindow window(sf::VideoMode(width,height), "Window Title", sf::Style::Titlebar | sf::Style::Close);
    //win = &window;
    PhysicsWorld::getInstance()->window = &window;
    PhysicsWorld::getInstance()->gravity = pum::vector2d(0,100);
    ImGui::SFML::Init(window);
    vector<Circle*> circles;
    vector<Rectangle*> rectangles;
    vector<Polygon*> polygons;


    std::vector<sf::Color>colorlist = {
        sf::Color::Blue,
        sf::Color::Cyan,
        sf::Color::Green,
        sf::Color::Magenta,
        sf::Color::Red,
        //sf::Color::White,
        sf::Color::Yellow
    };


    Rectangle* bar = new Rectangle(sf::Vector2f(450, 450), 300, 50, 1000, false, RigidBody::Static);
    rectangles.push_back(bar);
    bar->physicsbody->mask.reset();
    bar->physicsbody->layer.reset();

    Rectangle* bar2 = new Rectangle(sf::Vector2f(450, 490), 20, 80, 1000, false);
    bar2->physicsbody->velocity = pum::vector2d(0, 0);
    rectangles.push_back(bar2);

    spring* s1 = new spring(bar->physicsbody,pum::vector2d(100,0),bar2->physicsbody,pum::vector2d(0,40));
    s1->minlen = 100;
    s1->length = 150;
    s1->maxlen = 200;

    PhysicsWorld::getInstance()->springs.push_back(s1);

    




   


    //forming boundaries////////////////////
    Rectangle* temprectangle2 = new Rectangle(sf::Vector2f(450, 900), 800, 100, 10, false,RigidBody::Static,sf::Color::Transparent);
    Rectangle* temprectangle3 = new Rectangle(sf::Vector2f(900, 450), 100, 800, 10, false, RigidBody::Static,sf::Color::Transparent);
    Rectangle* temprectangle4 = new Rectangle(sf::Vector2f(450, 0), 800, 100, 10, false, RigidBody::Static,sf::Color::Transparent);
    Rectangle* temprectangle5 = new Rectangle(sf::Vector2f(0, 450), 100, 800, 10, false, RigidBody::Static,sf::Color::Transparent);
    rectangles.push_back(temprectangle2);
    rectangles.push_back(temprectangle3);
    rectangles.push_back(temprectangle4);
    rectangles.push_back(temprectangle5);

    sf::Clock gameClock;
    sf::Clock deltaClock;
    /*while (gameClock.getElapsedTime().asSeconds() < 13)
    {
        
    }
    gameClock.restart();*/
    while (window.isOpen())
    {
        window.clear(sf::Color::Black); //clearing the window


        for (auto& it : circles)
        {
            it->draw(window);
        }
        for (auto& it : rectangles)
        {
            it->draw(window);
        }
        for (auto& it : polygons)
        {
            it->draw(window);
        }

        //calling the process functions
        sf::Time deltatime = gameClock.getElapsedTime(); //deltatime
        double usec = deltatime.asMicroseconds();
        gameClock.restart(); //timer restart
        //std::cout << (usec / 1000000.000) << std::endl;
        double deltatimeconst = (usec / 1000000.00);// 0.0005;
        //double deltatimeconst = 0.05;
        //std::cout << deltatimeconst << std::endl;
        PhysicsWorld::getInstance()->process(deltatimeconst);

        for (auto& it : circles)
        {
            it->process(deltatimeconst);
        }
        for (auto& it : rectangles)
        {
            it->process(deltatimeconst);
        }
        for (auto& it : polygons)
        {
            it->process(deltatimeconst);
        }



     
        //getting event
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {
                cout << "close button clicked " << endl;
                window.close();
            }

        }
        
        
        ImGui::SFML::Update(window, deltaClock.restart());
        

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();


}

