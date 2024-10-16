#include "../header/RigidCircle.h"


RigidCircle::RigidCircle(sf::Vector2f pos,double mass,double radius, sf::Vector2f velocity, sf::Vector2f acceleration)
{
	this->pos = pos;
	this->mass = mass;
	this->radius = radius;
	this->velocity = velocity;
	this->velocity = sf::Vector2f(-200,200);
	this->acceleration = acceleration;
}
