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
    //PhysicsWorld::getInstance()->gravity = pum::vector2d(0, 1000);
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

    
    //drawing the engine

    //drawing the center of the wheel
    /*Circle* axel = new Circle(sf::Vector2f(450, 450), 20, 100, false, RigidBody::Static);
    circles.push_back(axel);
    axel->physicsbody->mask.reset();
    axel->physicsbody->layer.reset();*/

    Rectangle* rod1 = new Rectangle(sf::Vector2f(450,450), 80,4, 100, false,RigidBody::Static);
    rod1->physicsbody->angularvelocity = 200;
    rectangles.push_back(rod1);
    rod1->physicsbody->mask.reset();
    rod1->physicsbody->layer.reset(); 
    

    Rectangle* rod2 = new Rectangle(sf::Vector2f(450 + 20, 450 + 2), 4, 300, 100, false);
    rectangles.push_back(rod2);
    rod2->physicsbody->mask.reset();
    rod2->physicsbody->layer.reset();
     

    //joint between rod1 and rod2
    joint* j = new joint(10, false);
    j->staticfricitonconst = 0.00;
    PhysicsWorld::getInstance()->joints.push_back(j);
    j->addbodies(rod1->physicsbody, pum::vector2d(40, 0));
    j->addbodies(rod2->physicsbody, pum::vector2d(0,150));


    //adding piston
    Rectangle* piston = new Rectangle(sf::Vector2f(450,450-80-50),80,50, 100, false);
    rectangles.push_back(piston);

    //joint between piston and rod2
    joint* j1 = new joint(10, false);
    j1->staticfricitonconst = 0.00;
    PhysicsWorld::getInstance()->joints.push_back(j1);
    j1->addbodies(piston->physicsbody, pum::vector2d(0, 0));
    j1->addbodies(rod2->physicsbody, pum::vector2d(0, -150));


    //adding the walls of the engine
    Rectangle* wallleft = new Rectangle(sf::Vector2f(450 - 40 - 2, 150), 4, 200, 100, false,RigidBody::Static);
    rectangles.push_back(wallleft);

    Rectangle* wallright = new Rectangle(sf::Vector2f(450 + 40 + 2, 150), 4, 200, 100, false,RigidBody::Static);
    rectangles.push_back(wallright);




   


    //forming boundaries////////////////////
    /*Rectangle* temprectangle2 = new Rectangle(sf::Vector2f(450, 900), 800, 100, 10, false,RigidBody::Static);
    Rectangle* temprectangle3 = new Rectangle(sf::Vector2f(900, 450), 100, 800, 10, false, RigidBody::Static);
    Rectangle* temprectangle4 = new Rectangle(sf::Vector2f(450, 0), 800, 100, 10, false, RigidBody::Static);
    Rectangle* temprectangle5 = new Rectangle(sf::Vector2f(0, 450), 100, 800, 10, false, RigidBody::Static);
    rectangles.push_back(temprectangle2);
    rectangles.push_back(temprectangle3);
    rectangles.push_back(temprectangle4);
    rectangles.push_back(temprectangle5);*/

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

