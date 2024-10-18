#include "../header/staticball.h"
#include <iostream>


staticball::staticball(double x, double y, double radius, sf::Color color)
{
	this->shape.setOrigin(radius, radius);
	this->shape.setRadius(radius);
	this->shape.setPosition(x, y);
	this->shape.setFillColor(color);
	this->shape.setPointCount(20);
	this->physicsbody = new RigidCircle(sf::Vector2f(x, y),0, radius, RigidCircle::bodytype::_static);
	PhysicsEngine* engine = PhysicsEngine::getInstance();
	//engine->testingengine();
	int temp = PhysicsEngine::getInstance()->addObject(this->physicsbody);
}

staticball::~staticball()
{
	//add deconstructor code here
}

void staticball::draw(sf::RenderWindow& window)
{
	//std::cout << "inside draw of staticball " << std::endl;
	window.draw(this->shape);
}

void staticball::process(double delta)
{
	this->shape.setPosition(this->physicsbody->pos.x, this->physicsbody->pos.y);
	//std::cout<<"graphics pos " << this->shape.getPosition().x << " " << this->shape.getPosition().y << std::endl;
}