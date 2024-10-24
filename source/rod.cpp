#include "../header/rod.h"
#include <iostream>

Rod::Rod(double x, double y, double len, double radius,double angle)
{
	double dis = 0;
	this->len = len;
	this->radius = radius;
	this->x = x;
	this->y = y;
	this->degrees = angle;
	double torad = (this->degrees * 3.14159) / 180;;
	for (int i = 0; i < this->len; i++)
	{
		sf::CircleShape shape;
		shape.setOrigin(radius, radius);
		shape.setRadius(radius);
		//shape.setPosition(x + (dis * 1.41), x + (dis * 1.41));
		shape.setPosition(x + (dis*cos(torad)), x + (dis*(torad)));
		shape.setFillColor(sf::Color::Red);
		shape.setPointCount(20);
		this->shapes.push_back(shape);
		RigidCircle* circle = new RigidCircle(sf::Vector2f(x + (dis * 1.41), y + (dis * 1.41)), 10, radius, RigidCircle::bodytype::_static);
		this->balls.push_back(circle);
		int temp = PhysicsEngine::getInstance()->addObject(circle);
		dis = dis + (2*radius);
		
	}
}

Rod::~Rod()
{
	//add deconstructor code here
}

void Rod::draw(sf::RenderWindow& window)
{
	//std::cout << "inside draw of Rod " << std::endl;
	for (int i = 0; i < this->len; i++)
	{
		window.draw(this->shapes[i]);
	}
}

void Rod::process(double delta)
{
	/*continue;*/
	this->degrees += 0.2;
	double inrad = (this->degrees * 3.14159) / 180;
	int dis = 0;
	for (int i = 0; i < this->len; i++)
	{

		/*sf::Vector2f pos = this->shapes[i].getPosition();
		this->balls[i]->pos.x = this->x + (dis * cos(inrad));
		this->balls[i]->pos.y = this->y + (dis * sin(inrad));
		this->shapes[i].setPosition(this->balls[i]->pos.x, this->balls[i]->pos.y);
		dis = dis + (2 * this->radius);*/

		//this->shape.setPosition(this->physicsbody->pos.x, this->physicsbody->pos.y);
		this->shapes[i].setPosition(this->balls[i]->pos.x, this->balls[i]->pos.y);
		sf::Vector2f pos = this->shapes[i].getPosition();
		sf::Vector2f newpos;
		newpos.x = this->x + (dis * cos(inrad));
		newpos.y = this->y + (dis * sin(inrad));
		this->balls[i]->velocity.x = (newpos.x - pos.x)/delta;
		this->balls[i]->velocity.y = (newpos.y - pos.y)/delta;
		dis = dis + (2 * this->radius);
	}
	//std::cout<<"graphics pos " << this->shape.getPosition().x << " " << this->shape.getPosition().y << std::endl;
}