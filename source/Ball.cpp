#include "../header/Ball.h"
#include <iostream>

Ball::Ball(double x, double y, double radius, double mass, sf::Color color)
{
	this->shape.setOrigin(radius,radius);
	this->shape.setRadius(radius);
	this->shape.setPosition(x, y);
	this->shape.setFillColor(color);
	this->shape.setPointCount(20);
	this->physicsbody = new RigidCircle(sf::Vector2f(x, y), mass, radius,RigidCircle::bodytype::_rigid);
	PhysicsEngine* engine =  PhysicsEngine::getInstance();
	//engine->testingengine();
	int temp = PhysicsEngine::getInstance()->addObject(this->physicsbody);
}

Ball::~Ball()
{
	//add deconstructor code here
}

void Ball::draw(sf::RenderWindow& window)
{
	window.draw(this->shape);
}

void Ball::process(double delta)
{
	this->shape.setPosition(this->physicsbody->pos.x, this->physicsbody->pos.y);
}