#include "../header/RigidBody.h"

RigidBody::RigidBody(double mass, sf::Vector2f velocity, sf::Vector2f acceleration,type bodytype,double coefficientOfStaticFriction,double coefficientOfKineticFriction)
{
	this->mass = mass;
	this->coefficientOfStaticFriction = coefficientOfStaticFriction;
	this->coefficientOfKineticFriction = coefficientOfKineticFriction;
	this->bodyType = bodytype;
}
