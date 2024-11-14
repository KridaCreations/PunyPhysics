#include "../header/RigidBody.h"

RigidBody::RigidBody(double mass, pum::vector2d velocity, pum::vector2d acceleration,type bodytype,double coefficientOfStaticFriction,double coefficientOfKineticFriction)
{
	this->mass = mass;
	this->coefficientOfStaticFriction = coefficientOfStaticFriction;
	this->coefficientOfKineticFriction = coefficientOfKineticFriction;
	this->bodyType = bodytype;
	mask.set(1);
	layer.set(1);
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

long double RigidBody::deg2rad(double deg)
{
	return (deg /180.00) * 3.14159265358979323846;
}

long double RigidBody::rad2deg(double rad)
{
	return (rad / 3.14159265358979323846) * 180.00;
}

pum::vector2d RigidBody::getglobalpos(pum::vector2d local_pos)
{
	
	double rad = pum::deg2rad(this->angle);
	
	//std::cout << __FUNCTION__ << " " << __LINE__ << " angle in degrees "<<(this->angle)<< std::endl;
	//std::cout << __FUNCTION__ << " " << __LINE__ << " " << rad << std::endl;
	local_pos = pum::vector2d(((local_pos.x * cos(rad)) - (local_pos.y * sin(rad))), ((local_pos.x * sin(rad)) + (local_pos.y * cos(rad))));
	//std::cout << __FUNCTION__ << " " << local_pos.x << " " << local_pos.y << std::endl;
	local_pos = local_pos + this->position;
	if (abs(local_pos.x) < 0.00005)
	{	
		local_pos.x = 0;
	}
	if (abs(local_pos.y) < 0.00005)
	{
		local_pos.y = 0;
	}
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