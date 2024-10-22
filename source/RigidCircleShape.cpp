#include "../header/RigidCircleShape.h"

RigidCircleShape::RigidCircleShape(sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f acceleration, double radius, double mass, type bodytype, double coefficientOfStaticFriction, double coefficientOfKineticFriction)
	:RigidBody(mass,velocity,acceleration, bodytype, coefficientOfStaticFriction, coefficientOfKineticFriction)
{
	this->radius = radius;
	this->momentOfInertia = (mass * (radius * radius)) / 2;
	this->shape.setOrigin(sf::Vector2f(radius, radius));
	this->shape.setRadius(radius);
	this->shape.setPosition(pos);
	this->shape.setPointCount(3); 

	this->shapetype = RigidBody::shapetypes::Circle;
}
