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

pum::vector2d RigidBody::getglobalpos(pum::vector2d local_pos)
{
	
	double rad = pum::deg2rad(this->angle);
	
	//std::cout << __FUNCTION__ << " " << __LINE__ << " angle in degrees "<<(this->angle)<< std::endl;
	//std::cout << __FUNCTION__ << " " << __LINE__ << " " << rad << std::endl;
	local_pos = pum::vector2d(((local_pos.x * cos(rad)) - (local_pos.y * sin(rad))), ((local_pos.x * sin(rad)) + (local_pos.y * cos(rad))));
	//std::cout << __FUNCTION__ << " " << local_pos.x << " " << local_pos.y << std::endl;
	local_pos = local_pos + this->position;
	return local_pos;
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