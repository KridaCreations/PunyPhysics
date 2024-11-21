#include "../header/Circle.h"
#include <iostream>

Circle::Circle(sf::Vector2f pos, double radius, double mass, bool control,RigidBody::type bodytype, sf::Color color)
{
	this->physicsbody = new RigidCircleShape(pum::vector2d(pos.x, pos.y), pum::vector2d(), pum::vector2d(), radius, mass, bodytype);
	
	//this->shape.setFillColor(color);
	this->shape.setFillColor(sf::Color::Transparent);
	this->shape.setOrigin(sf::Vector2f(radius, radius));
	this->shape.setRadius(radius);
	this->shape.setOutlineThickness(2);
	this->shape.setPointCount(8);
	this->control = control;
	PhysicsWorld::getInstance()->addbody(this->physicsbody);
}

Circle::~Circle()
{

}

void Circle::draw(sf::RenderWindow& window)
{
	window.draw(this->shape);
}

void Circle::process(double delta)
{
	//retrieving the position and rotation from physics world
	sf::Vector2f sfvect(this->physicsbody->position.x, this->physicsbody->position.y);
	this->shape.setPosition(sfvect);
	this->shape.setRotation(this->physicsbody->angle);
	if (this->physicsbody->iscolliding)
	{
		this->shape.setOutlineColor(sf::Color::White);
		//std::cout << "true" << std::endl;
	}
	else
	{
		//std::cout << "flase" << std::endl;
		//this->shape.setOutlineColor(sf::Color::Transparent);
		this->shape.setOutlineColor(sf::Color::White);
	}

	if (this->control)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			physicsbody->velocity = pum::vector2d(-100, 0);// sf::Vector2f(-100, 0);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			physicsbody->velocity = pum::vector2d(100, 0);// sf::Vector2f(100, 0);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			physicsbody->velocity = pum::vector2d(0, -100);// sf::Vector2f(0, -100);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			physicsbody->velocity = pum::vector2d(0, 100);// sf::Vector2f(0, 100);
		else
			physicsbody->velocity = pum::vector2d();// sf::Vector2f(0, 0);

		
	}
}