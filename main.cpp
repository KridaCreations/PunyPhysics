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
    int width = 1080;
    int height = 900;
    sf::RenderWindow window(sf::VideoMode(width,height), "Window Title", sf::Style::Titlebar | sf::Style::Close);
    //win = &window;
    PhysicsWorld::getInstance()->window = &window;
    //PhysicsWorld::getInstance()->gravity = pum::vector2d(0,1000);
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


    Rectangle* wiperleftbrush = new Rectangle(sf::Vector2f(450 - 200, 450), 10, 200, 100, false, RigidBody::Rigid, sf::Color::Magenta);
    rectangles.push_back(wiperleftbrush);
    wiperleftbrush->physicsbody->layer.reset();
    wiperleftbrush->physicsbody->setrotation(-50);


    Rectangle* wiperleft = new Rectangle(sf::Vector2f(450 - 200, 450), 10, 200, 100, false, RigidBody::Rigid, sf::Color::Red);
    rectangles.push_back(wiperleft);
    wiperleft->physicsbody->layer.reset();


    joint* jbrushleft = new joint(10, false);
    jbrushleft->staticfricitonconst = 1000;
    PhysicsWorld::getInstance()->joints.push_back(jbrushleft);
    jbrushleft->addbodies(wiperleft->physicsbody, pum::vector2d(0, -70));
    jbrushleft->addbodies(wiperleftbrush->physicsbody, pum::vector2d(0, 40));

    //wiperleft->physicsbody->angularvelocity = 300;
    Rectangle* wiperleftanchor = new Rectangle(sf::Vector2f(450 - 200, 450), 10, 10, 10, false, RigidBody::Static, sf::Color::White);
    rectangles.push_back(wiperleftanchor);
    wiperleftanchor->physicsbody->layer.reset();


    joint* j1 = new joint(10, false);
    j1->staticfricitonconst = 0.0;
    PhysicsWorld::getInstance()->joints.push_back(j1);
    j1->addbodies(wiperleft->physicsbody, pum::vector2d(0, 0));
    j1->addbodies(wiperleftanchor->physicsbody, pum::vector2d(0, 0));



    Rectangle* wiperrightbrush = new Rectangle(sf::Vector2f(450 - 200, 450), 10, 200, 100, false, RigidBody::Rigid, sf::Color::Magenta);
    rectangles.push_back(wiperrightbrush);
    wiperrightbrush->physicsbody->layer.reset();
    wiperrightbrush->physicsbody->setrotation(-50);

    Rectangle* wiperright = new Rectangle(sf::Vector2f(450 + 200, 450), 10, 200, 100, false, RigidBody::Rigid, sf::Color::Red);
    rectangles.push_back(wiperright);
    wiperright->physicsbody->layer.reset();

    joint* jbrushright = new joint(10, false);
    jbrushright->staticfricitonconst = 1000;
    PhysicsWorld::getInstance()->joints.push_back(jbrushright);
    jbrushright->addbodies(wiperright->physicsbody, pum::vector2d(0, -70));
    jbrushright->addbodies(wiperrightbrush->physicsbody, pum::vector2d(0, 40));

    Rectangle* wiperrightanchor = new Rectangle(sf::Vector2f(450 + 200, 450), 10, 10, 10, false, RigidBody::Static, sf::Color::White);
    rectangles.push_back(wiperrightanchor);
    wiperrightanchor->physicsbody->layer.reset();


    joint* j2 = new joint(10, false);
    j2->staticfricitonconst = 0.0;
    PhysicsWorld::getInstance()->joints.push_back(j2);
    j2->addbodies(wiperright->physicsbody, pum::vector2d(0, 0));
    j2->addbodies(wiperrightanchor->physicsbody, pum::vector2d(0, 0));



    Rectangle* rod1 = new Rectangle(sf::Vector2f(450, 450), 400, 10, 10, false, RigidBody::Rigid, sf::Color::Green);
    rectangles.push_back(rod1);
    //rod1->physicsbody->velocity = pum::vector2d(200, 0);
    rod1->physicsbody->layer.reset();

    joint* j3 = new joint(10, false);
    j3->staticfricitonconst = 0.00;
    PhysicsWorld::getInstance()->joints.push_back(j3);
    j3->addbodies(wiperleft->physicsbody, pum::vector2d(0, 50));
    j3->addbodies(rod1->physicsbody, pum::vector2d(-200, 0));


    joint* j4 = new joint(10, false);
    j4->staticfricitonconst = 0.00;
    PhysicsWorld::getInstance()->joints.push_back(j4);
    j4->addbodies(wiperright->physicsbody, pum::vector2d(0, 50));
    j4->addbodies(rod1->physicsbody, pum::vector2d(200, 0));


    pum::vector2d to = (pum::vector2d(570, 600) + pum::vector2d(-40, 0));
    pum::vector2d from = wiperright->physicsbody->position + pum::vector2d(0, 100);
    double length = (to-from).length();
    pum::vector2d mid = (to - from) / 2;
    cout << "length " << length << endl;


    Rectangle* rod2 = new Rectangle(sf::Vector2f(mid.x,mid.y), 10, length, 10, false, RigidBody::Rigid, sf::Color::Green);
    rectangles.push_back(rod2);
    rod2->physicsbody->layer.reset();
    rod2->physicsbody->setrotation(63);


    joint* j5 = new joint(10, false);
    j5->staticfricitonconst = 0.00;
    PhysicsWorld::getInstance()->joints.push_back(j5);
    j5->addbodies(wiperright->physicsbody, pum::vector2d(0, 100));
    j5->addbodies(rod2->physicsbody, pum::vector2d(0, (length / (-2))));


    Circle* axel = new Circle(sf::Vector2f(570, 600), 40, 100, false, RigidBody::Static,sf::Color::Blue);
    circles.push_back(axel);
    axel->physicsbody->layer.reset();
    axel->physicsbody->angularvelocity = 400;

    joint* j6 = new joint(10, false);
    j6->staticfricitonconst = 0.00;
    PhysicsWorld::getInstance()->joints.push_back(j6);
    j6->addbodies(axel->physicsbody, pum::vector2d(-40, 0));
    j6->addbodies(rod2->physicsbody, pum::vector2d(0, (length/2)));










    //forming boundaries////////////////////
    /*Rectangle* temprectangle2 = new Rectangle(sf::Vector2f(450, 900), 800, 100, 10, false,RigidBody::Static,sf::Color::Transparent);
    Rectangle* temprectangle3 = new Rectangle(sf::Vector2f(900, 450), 100, 800, 10, false, RigidBody::Static,sf::Color::Transparent);
    Rectangle* temprectangle4 = new Rectangle(sf::Vector2f(450, 0), 800, 100, 10, false, RigidBody::Static,sf::Color::Transparent);
    Rectangle* temprectangle5 = new Rectangle(sf::Vector2f(0, 450), 100, 800, 10, false, RigidBody::Static,sf::Color::Transparent);
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

