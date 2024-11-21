//#include "imgui.h"
//#include "imgui-SFML.h"
//
//#include "SFML/Graphics.hpp"
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(800, 800), "Window Title");
//    ImGui::SFML::Init(window);
//
//    bool circleExists = true;
//    float circleRadius = 200.0f;
//    int circleSegments = 100;
//    float circleColor[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
//    sf::CircleShape shape(circleRadius, circleSegments);
//    shape.setFillColor(sf::Color
//    (
//        (int)(circleColor[0] * 255),
//        (int)(circleColor[1] * 255),
//        (int)(circleColor[2] * 255)
//    )); // Color circle
//    shape.setOrigin(circleRadius, circleRadius);
//    shape.setPosition(400, 400); // Center circle
//
//    sf::Clock deltaClock;
//    int counter = 0;
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            ImGui::SFML::ProcessEvent(event);
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//        ImGui::SFML::Update(window, deltaClock.restart());
//
//        ImGui::Begin("Window title");
//        ImGui::Text("Window text!");
//        ImGui::Checkbox("Circle", &circleExists);
//        ImGui::SliderFloat("Radius", &circleRadius, 100.0f, 300.0f);
//        ImGui::SliderInt("Sides", &circleSegments, 3, 150);
//        ImGui::ColorEdit3("Color Circle", circleColor);
//        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//            counter++;
//        if (ImGui::Button("Button2"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//            counter++;
//        ImGui::SameLine();
//        ImGui::Text("counter = %d", counter);
//        ImGui::End();
//
//        shape.setRadius(circleRadius);
//        shape.setOrigin(circleRadius, circleRadius);
//        shape.setPointCount(circleSegments);
//        shape.setFillColor(sf::Color
//        (
//            (int)(circleColor[0] * 255),
//            (int)(circleColor[1] * 255),
//            (int)(circleColor[2] * 255)
//        )); // Color circle
//        window.clear(sf::Color(00,00,00)); // Color background
//        if (circleExists)
//            window.draw(shape);
//        ImGui::SFML::Render(window);
//        
//        window.display();
//    }
//
//    ImGui::SFML::Shutdown();
//    return 0;
//}













///////////////////// to use for later

/*


for (int i = 0; i < 3; i++)
    {
        int x = rand() % 400 + 100;
        int y = rand() % 400 + 100;
        int col = rand() % colorlist.size();
        Circle*  temp = new Circle(sf::Vector2f(x,y), 20, 100, false, RigidBody::Rigid,colorlist[col]);
        circles.push_back(temp);
    }

    for (int i = 0; i < 3; i++)
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

    for (int i = 0; i < 3; i++)
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
            //std::cout << temp.x << " " << temp.y << std::endl;
            double rad = ((360/sides) * 3.14) / 180.0;// this->angle);
            temp = sf::Vector2f(((temp.x * cos(rad)) - (temp.y * sin(rad))), ((temp.x * sin(rad)) + (temp.y * cos(rad))));
        }
        Polygon* tempbody = new Polygon(sf::Vector2f(x, y), p, 40, false,RigidBody::Rigid, colorlist[col]);
        polygons.push_back(tempbody);
    }


    Rectangle* bar = new Rectangle(sf::Vector2f(450, 450), 300, 50, 1000, false, RigidBody::Static);
    bar->physicsbody->angularvelocity = -40;
    //bar->physicsbody->velocity = sf::Vector2f(0, -10.0);
    //bar->physicsbody->rotate(20);
    rectangles.push_back(bar);
    bar->physicsbody->mask.reset();
    bar->physicsbody->layer.reset();

    joint* j = new joint(10, false);
    j->staticfricitonconst = 0.00;
    PhysicsWorld::getInstance()->joints.push_back(j);

    Rectangle* bar2 = new Rectangle(sf::Vector2f(600,490), 20, 80, 100, false);
    bar2->physicsbody->velocity = pum::vector2d(1000,0);
    rectangles.push_back(bar2);

    j->addbodies(bar->physicsbody, pum::vector2d(150, -25));
    j->addbodies(bar2->physicsbody, pum::vector2d(-10,-40));


    if (true)
    {

        joint* j2 = new joint(10, false);
        j2->staticfricitonconst = 0.3;// 99999999999.05;
        PhysicsWorld::getInstance()->joints.push_back(j2);

        Rectangle* bar3 = new Rectangle(sf::Vector2f(600, 490), 20, 80, 10, false);
        rectangles.push_back(bar3);

        j2->addbodies(bar2->physicsbody, pum::vector2d(10,40));
        j2->addbodies(bar3->physicsbody, pum::vector2d(-10, -40));


        joint* j3 = new joint(10, false);
        j3->staticfricitonconst = 0.3;// 99999999999.05;
        PhysicsWorld::getInstance()->joints.push_back(j3);

        Rectangle* bar4 = new Rectangle(sf::Vector2f(600, 490), 20, 80, 10, false);
        rectangles.push_back(bar4);

        j3->addbodies(bar3->physicsbody, pum::vector2d(10, 40));
        j3->addbodies(bar4->physicsbody, pum::vector2d(-10, -40));

        joint* j4 = new joint(10, false);
        j4->staticfricitonconst = 0.3;// 99999999999.05;
        PhysicsWorld::getInstance()->joints.push_back(j4);

        Rectangle* bar5 = new Rectangle(sf::Vector2f(600, 490), 20, 80, 10, false);
        rectangles.push_back(bar5);

        j4->addbodies(bar4->physicsbody, pum::vector2d(10, 40));
        j4->addbodies(bar5->physicsbody, pum::vector2d(-10, -40));
    }


*/


/*

//drawing the engine

    //drawing the center of the wheel

    Rectangle* rod1 = new Rectangle(sf::Vector2f(450,450), 80,10, 100, false,RigidBody::Static,sf::Color::Green);
    rod1->physicsbody->angularvelocity = 500;
    rectangles.push_back(rod1);
    rod1->physicsbody->mask.reset();
    rod1->physicsbody->layer.reset();
    //

    Rectangle* rod2 = new Rectangle(sf::Vector2f(450 + 20, 450 + 2), 10, 300, 100, false,RigidBody::Rigid,sf::Color::White);
    rectangles.push_back(rod2);
    rod2->physicsbody->mask.reset();
    rod2->physicsbody->layer.reset();
    //

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


    ////adding the walls of the engine
    Rectangle* wallleft = new Rectangle(sf::Vector2f(450 - 40 - 5, 150), 10, 200, 100, false,RigidBody::Static,sf::Color::Yellow);
    rectangles.push_back(wallleft);

    Rectangle* wallright = new Rectangle(sf::Vector2f(450 + 40 + 5, 150), 10, 200, 100, false,RigidBody::Static, sf::Color::Yellow);
    rectangles.push_back(wallright);




*/



/*
//spring simulations
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




*/