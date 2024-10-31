#include "../header/RigidRectangelShape.h"
#include <iostream>
#include "../header/PhysicsWorld.h"

RigidRectangleShape::RigidRectangleShape(sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f acceleration,double width,double length,double mass,type bodytype, double coefficientOfStaticFriction , double coefficientOfKineticFriction)
	:RigidBody(mass,velocity,acceleration, bodytype, coefficientOfStaticFriction, coefficientOfKineticFriction)
{

	this->length = length;
	this->width = width;
	this->momentOfInertia = (mass * ((width * width) + (length * length))) / 12.0;
	this->shapetype = RigidBody::shapetypes::Rectangle;
	
	this->position = pos;
	this->points[0] = sf::Vector2f(pos.x - (width / 2.0), pos.y - (length / 2.0));
	this->points[1] = sf::Vector2f(pos.x + (width / 2.0), pos.y - (length / 2.0));
	this->points[2] = sf::Vector2f(pos.x + (width / 2.0), pos.y + (length / 2.0));
	this->points[3] = sf::Vector2f(pos.x - (width / 2.0), pos.y + (length / 2.0));
	
	this->angle = 0;

}

void RigidRectangleShape::rotate(double angle)
{
	this->angle = this->angle + angle;

	sf::Vector2f newpos = this->position;
	this->points[0] = sf::Vector2f(newpos.x - (width / 2.0), newpos.y - (length / 2.0));
	this->points[1] = sf::Vector2f(newpos.x + (width / 2.0), newpos.y - (length / 2.0));
	this->points[2] = sf::Vector2f(newpos.x + (width / 2.0), newpos.y + (length / 2.0));
	this->points[3] = sf::Vector2f(newpos.x - (width / 2.0), newpos.y + (length / 2.0));


	//again rotating by the body angle
	double rad = this->deg2rad(this->angle);
	for (int point = 0; point < this->points.size(); point++)
	{
		this->points[point] = this->points[point] - this->position;
		sf::Vector2f temp = this->points[point];
		this->points[point] = sf::Vector2f(((temp.x * cos(rad)) - (temp.y * sin(rad))), ((temp.x * sin(rad)) + (temp.y * cos(rad))));
		this->points[point] = this->points[point] + this->position;
	}
	//std::cout << std::endl;
}

void RigidRectangleShape::translate(sf::Vector2f pos)
{
	this->position = this->position + pos;//changing the position

	//freshly deciding the body position
	sf::Vector2f newpos = this->position;
	this->points[0] = sf::Vector2f(newpos.x - (width / 2.0), newpos.y - (length / 2.0));
	this->points[1] = sf::Vector2f(newpos.x + (width / 2.0), newpos.y - (length / 2.0));
	this->points[2] = sf::Vector2f(newpos.x + (width / 2.0), newpos.y + (length / 2.0));
	this->points[3] = sf::Vector2f(newpos.x - (width / 2.0), newpos.y + (length / 2.0));


	//again rotating by the body angle
	double rad = this->deg2rad(this->angle);
	for (int point = 0; point < this->points.size(); point++)
	{
		this->points[point] = this->points[point] - this->position;
		sf::Vector2f temp = this->points[point];
		this->points[point] = sf::Vector2f(((temp.x * cos(rad)) - (temp.y * sin(rad))), ((temp.x * sin(rad)) + (temp.y * cos(rad))));
		this->points[point] = this->points[point] + this->position;
	}
}

void RigidRectangleShape::setpositon(sf::Vector2f pos)
{
	for (int point = 0; point < this->points.size(); point++)
	{
		this->points[point] = this->points[point] + (pos - this->position);
	}
	this->position = pos;
}

void RigidRectangleShape::setrotation(double angle)
{
	this->rotate(-1 * this->angle);
	this->rotate(angle);
	this->angle = angle;
}