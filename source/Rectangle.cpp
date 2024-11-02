#include "../header/Rectangle.h"
#include <iostream>

Rectangle::Rectangle(sf::Vector2f pos, double width, double length, double mass, bool control, RigidBody::type bodytype, sf::Color color)
{
	physicsbody = new RigidRectangleShape(pum::vector2d(pos.x,pos.y),pum::vector2d(0,0), pum::vector2d(),width,length,mass,bodytype);
	this->shape.setFillColor(color);
	this->shape.setOrigin(sf::Vector2f(width / 2, length / 2));
	this->shape.setSize(sf::Vector2f(width,length));
	this->shape.setOutlineThickness(2);
	this->control = control;
	PhysicsWorld::getInstance()->addbody(this->physicsbody);
}

Rectangle::~Rectangle()
{

}

void Rectangle::draw(sf::RenderWindow& window)
{
	this->boundbox.setPointCount(4);
	for (int point = 0; point < this->physicsbody->points.size(); point++)
	{
		sf::Vector2f sfvect(this->physicsbody->points[point].x, this->physicsbody->points[point].y);
		this->boundbox.setPoint(point,sfvect);
	}
	this->boundbox.setFillColor(sf::Color::Transparent);
	this->boundbox.setOutlineColor(sf::Color::White);
	this->boundbox.setOutlineThickness(2);
	//window.draw(this->shape);
	window.draw(this->boundbox);
}

void Rectangle::process(double delta)
{
	//retireving the position and rotation from physics world
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
	{
		
	}
	this->shape.setRotation(this->physicsbody->angle);
	sf::Vector2f sfvect(this->physicsbody->position.x, this->physicsbody->position.y);
	this->shape.setPosition(sfvect);
	if (this->physicsbody->iscolliding)
	{
		//this->shape.setOutlineColor(sf::Color::White);

	}
	else
	{
		this->shape.setOutlineColor(sf::Color::Transparent);
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			this->physicsbody->rotate(0.1);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->physicsbody->rotate(-0.1);
		

	}
}