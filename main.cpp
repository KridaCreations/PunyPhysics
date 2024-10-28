#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "./header/PhysicsWorld.h"
#include "./header/PhysicsEngine.h"
#include "./header/Circle.h"
#include "./header/Rectangle.h"

#include <iostream>
#include<unordered_set>
using namespace std;

PhysicsWorld* PhysicsWorld::instancePtr = nullptr;
PhysicsEngine* PhysicsEngine::instancePtr = nullptr;

//sf::RenderWindow* win;

int main()
{
    int width = 900;
    int height = 900;
    sf::RenderWindow window(sf::VideoMode(width,height), "Window Title", sf::Style::Titlebar | sf::Style::Close);
    //win = &window;
    PhysicsWorld::getInstance()->window = &window;
    PhysicsWorld::getInstance()->gravity = sf::Vector2f(0, 1000);
    ImGui::SFML::Init(window);
    vector<Circle*> circles;
    vector<Rectangle*> rectangles;
    //Circle* tempcircle1= new Circle(sf::Vector2f(300, 700), 25, 10, false, RigidBody::Static);
    //tempcircle1->physicsbody->velocity += sf::Vector2f(100, 100);
    //Circle* tempcircle2 = new Circle(sf::Vector2f(300, 400), 25, 10, false, RigidBody::Rigid);
    //circles.push_back(tempcircle1);
    //circles.push_back(tempcircle2);

    //Rectangle* temprectangle = new Rectangle(sf::Vector2f(200, 200), 200, 50, 10, false, RigidBody::Rigid);
    //rectangles.push_back(temprectangle);
    //temprectangle->physicsbody->rotate(10);

    /*Rectangle* temprectangle6 = new Rectangle(sf::Vector2f(200, 400), 50, 50, 10, false, RigidBody::Static);
    rectangles.push_back(temprectangle6);
    temprectangle6->physicsbody->rotate(45);*/


    std::vector<sf::Color>colorlist = {
        sf::Color::Blue,
        sf::Color::Cyan,
        sf::Color::Green,
        sf::Color::Magenta,
        sf::Color::Red,
        //sf::Color::White,
        sf::Color::Yellow
    };

    for (int i = 0; i <10; i++)
    {
        int x = rand() % 500 + 100;
        int y = rand() % 500 + 100;
        int col = rand() % colorlist.size();
        Circle*  temp = new Circle(sf::Vector2f(x,y), 40, 10, false, RigidBody::Rigid,colorlist[col]);
        circles.push_back(temp);
    }

    for (int i = 0; i < 10; i++)
    {
        int x = rand() % 500 + 100;
        int y = rand() % 500 + 100;
        int col = rand() % colorlist.size();
        Rectangle* temp = new Rectangle(sf::Vector2f(x, y),80,80, 10, false, RigidBody::Rigid, colorlist[col]);
        temp->physicsbody->rotate(2);
        rectangles.push_back(temp);
    }

   /* Circle* ball = new Circle(sf::Vector2f(300, 200), 90, 10, false, RigidBody::Rigid,sf::Color::Red);
    circles.push_back(ball);*/

    /*Rectangle* rect = new Rectangle(sf::Vector2f(300, 300), 200, 40,10, false, RigidBody::Rigid, sf::Color::Red);
    rectangles.push_back(rect);*/

    Rectangle* bar = new Rectangle(sf::Vector2f(450, 450), 700, 50, 1000, false, RigidBody::Static);
    bar->physicsbody->angularvelocity = 10;
    //bar->physicsbody->velocity = sf::Vector2f(0, -10.0);
    rectangles.push_back(bar);


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

        //calling the process functions
        sf::Time deltatime = gameClock.getElapsedTime(); //deltatime
        double usec = deltatime.asMicroseconds();
        gameClock.restart(); //timer restart
        //std::cout << (usec / 1000000.000) << std::endl;
        double deltatimeconst = (usec / 1000000.00);// 0.0005;
        //double deltatimeconst = 0.0005;
        PhysicsWorld::getInstance()->process(deltatimeconst);

        for (auto& it : circles)
        {
            it->process(deltatimeconst);
        }
        for (auto& it : rectangles)
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
        //ImGui::Begin("Window title");
        
        //ImGui::End();
        //draw function
        
       
        

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();


}

