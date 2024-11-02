#include "../header/RigidCircleShape.h"

RigidCircleShape::RigidCircleShape(pum::vector2d pos, pum::vector2d velocity, pum::vector2d acceleration, double radius, double mass, type bodytype, double coefficientOfStaticFriction, double coefficientOfKineticFriction)
	:RigidBody(mass,velocity,acceleration, bodytype, coefficientOfStaticFriction, coefficientOfKineticFriction)
{
	this->radius = radius;
	this->momentOfInertia = (mass * (radius * radius)) / 2;

	this->shapetype = RigidBody::shapetypes::Circle;

	this->radius = radius;
	this->setpositon(pos);
	this->angle = 0;

}

void RigidCircleShape::rotate(double angle)
{
	this->angle = this->angle + angle;
}

void RigidCircleShape::translate(pum::vector2d pos)
{
	this->position = this->position + pos;
}

void RigidCircleShape::setpositon(pum::vector2d pos)
{
	this->position = pos;
}

void RigidCircleShape::setrotation(double angle)
{
	this->angle = angle;
}