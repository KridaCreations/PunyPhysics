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
    PhysicsWorld::getInstance()->gravity = pum::vector2d(0, 100);
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

    for (int i = 0; i < 0; i++)
    {
        int x = rand() % 400 + 100;
        int y = rand() % 400 + 100;
        int col = rand() % colorlist.size();
        Circle*  temp = new Circle(sf::Vector2f(x,y), 20, 100, false, RigidBody::Rigid,colorlist[col]);
        circles.push_back(temp);
    }

    for (int i = 0; i < 0; i++)
    {
        //int x = rand() % 500 + 100;
        //int y = rand() % 500 + 100;
        int x = 300;
        int y = 300;
        int col = rand() % colorlist.size();
        //std::cout << "adding a rect at " << x << " " << y << std::endl;
        Rectangle* temp = new Rectangle(sf::Vector2f(x, y),40,40, 100, false, RigidBody::Rigid, colorlist[col]);
        //temp->physicsbody->rotate(2);
        rectangles.push_back(temp);
    }
    
    for (int i = 0; i < 0; i++)
    {
        //int x = rand() % 500 + 100;
        //int y = rand() % 500 + 100;
        int x = 300;
        int y = 100;
        int col = rand() % colorlist.size();
        int radiilen = 10;
        pum::vector2d temppum(0, 1);
        temppum.makelen(30);
        sf::Vector2f temp(temppum.x, temppum.y);
        std:vector<sf::Vector2f>p;
        int sides = 6;
        for (int j = 0; j < sides; j++)
        {
            p.push_back(temp + sf::Vector2f(x, y));
            std::cout << temp.x << " " << temp.y << std::endl;
            double rad = ((360/sides) * 3.14) / 180.0;// this->angle);
            temp = sf::Vector2f(((temp.x * cos(rad)) - (temp.y * sin(rad))), ((temp.x * sin(rad)) + (temp.y * cos(rad))));
        }
        Polygon* tempbody = new Polygon(sf::Vector2f(x, y), p, 40, false,RigidBody::Rigid, colorlist[col]);
        polygons.push_back(tempbody);
    }

   /* Circle* ball = new Circle(sf::Vector2f(300, 200), 90, 10, false, RigidBody::Rigid,sf::Color::Red);
    circles.push_back(ball);*/

    /*Rectangle* rect = new Rectangle(sf::Vector2f(200, 100), 40, 50,10, false, RigidBody::Rigid, sf::Color::Red);
    rectangles.push_back(rect);*/

    Rectangle* bar = new Rectangle(sf::Vector2f(450, 450), 300, 50, 1000, false, RigidBody::Static);
    //bar->physicsbody->angularvelocity = 40;
    //bar->physicsbody->velocity = sf::Vector2f(0, -10.0);
    //bar->physicsbody->rotate(20);
    rectangles.push_back(bar);


    joint* j = new joint(10, false);
    PhysicsWorld::getInstance()->joints.push_back(j);

    Rectangle* bar3 = new Rectangle(sf::Vector2f(600,490), 20, 80, 1000, false);
    //bar3->physicsbody->angularvelocity = 40;
    bar3->physicsbody->velocity = pum::vector2d(100,0);
    //bar3->physicsbody->rotate(-45);
    rectangles.push_back(bar3);

    j->addbodies(bar->physicsbody, pum::vector2d(150, 0));
    j->addbodies(bar3->physicsbody, pum::vector2d(0,-40));



    joint* j2 = new joint(10, false);
    PhysicsWorld::getInstance()->joints.push_back(j2);

    Rectangle* bar4 = new Rectangle(sf::Vector2f(600, 490), 20, 80, 1000, false);
    //bar3->physicsbody->angularvelocity = 40;
    //bar3->physicsbody->velocity = pum::vector2d(100,0);
    //bar3->physicsbody->rotate(-45);
    rectangles.push_back(bar4);

    j2->addbodies(bar3->physicsbody, pum::vector2d(0,40));
    j2->addbodies(bar4->physicsbody, pum::vector2d(0, -40));


    joint* j3 = new joint(10, false);
    PhysicsWorld::getInstance()->joints.push_back(j3);

    Rectangle* bar5 = new Rectangle(sf::Vector2f(600, 490), 20, 80, 1000, false);
    //bar3->physicsbody->angularvelocity = 40;
    //bar3->physicsbody->velocity = pum::vector2d(100,0);
    //bar3->physicsbody->rotate(-45);
    rectangles.push_back(bar5);

    j3->addbodies(bar4->physicsbody, pum::vector2d(0, 40));
    j3->addbodies(bar5->physicsbody, pum::vector2d(0, -40));




    //Rectangle* bar2 = new Rectangle(sf::Vector2f(600, 600), 400, 50, 1000, false, RigidBody::Static);
    //bar2->physicsbody->angularvelocity = 40;
    ////bar->physicsbody->velocity = sf::Vector2f(0, -10.0);
    //bar2->physicsbody->rotate(-20);
    //rectangles.push_back(bar2);


    //forming boundaries////////////////////
    Rectangle* temprectangle2 = new Rectangle(sf::Vector2f(450, 900), 800, 100, 10, false,RigidBody::Static);
    Rectangle* temprectangle3 = new Rectangle(sf::Vector2f(900, 450), 100, 800, 10, false, RigidBody::Static);
    Rectangle* temprectangle4 = new Rectangle(sf::Vector2f(450, 0), 800, 100, 10, false, RigidBody::Static);
    Rectangle* temprectangle5 = new Rectangle(sf::Vector2f(0, 450), 100, 800, 10, false, RigidBody::Static);
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
        //double deltatimeconst = 0.0005;
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

