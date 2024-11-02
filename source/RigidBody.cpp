#include "../header/RigidBody.h"

RigidBody::RigidBody(double mass, pum::vector2d velocity, pum::vector2d acceleration,type bodytype,double coefficientOfStaticFriction,double coefficientOfKineticFriction)
{
	this->mass = mass;
	this->coefficientOfStaticFriction = coefficientOfStaticFriction;
	this->coefficientOfKineticFriction = coefficientOfKineticFriction;
	this->bodyType = bodytype;
}

double RigidBody::getInvMass()
{
	if (this->bodyType == RigidBody::Static)
		return 0;
	else
		return 1.0 / this->mass;
}

double RigidBody::getInvInertia()
{
	if (this->bodyType == RigidBody::Static)
		return 0;
	else
		return (1 / this->momentOfInertia);
}

double RigidBody::deg2rad(double deg)
{
	return (deg * 3.14)/180.0;
}

double RigidBody::rad2deg(double rad)
{
	return (rad * 180) / 3.14;
}


void RigidBody::rotate(double angle)
{
	
}

void RigidBody::translate(pum::vector2d pos)
{
	
}

void RigidBody::setpositon(pum::vector2d pos)
{
	
}

void RigidBody::setrotation(double angle)
{
	
}