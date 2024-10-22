#include "../header/RigidRectangelShape.h"

RigidRectangleShape::RigidRectangleShape(sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f acceleration,double width,double length,double mass,type bodytype = type::Rigid, double coefficientOfStaticFriction = 0 , double coefficientOfKineticFriction = 0)
	:RigidBody(mass,velocity,acceleration, bodytype, coefficientOfStaticFriction, coefficientOfKineticFriction)
{
	this->shape.setOrigin(width / 2, length / 2);
	this->shape.setPosition(pos);
	this->shape.setSize(sf::Vector2f(width, length));
	this->length = length;
	this->width = width;
	this->momentOfInertia = (mass * ((width * width) + (length * length))) / 12;
	this->shapetype = RigidBody::shapetypes::Rectangle;
}