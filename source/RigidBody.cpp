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

void RigidBody::applyImpulse(pum::vector2d impulse, pum::vector2d position)
{
	pum::vector2d r = position - this->position;
	pum::vector2d rperp = pum::vector2d(-1.0 * r.y, 1.0 * r.x);
	//this->velocity = this->velocity + (impulse * this->getInvMass());
	this->velocity = this->velocity + (impulse * this->getInvMass());
	//this->angularvelocity += this->rad2deg(pum::dotpro(rperp, impulse)) * this->getInvInertia();
	this->angularvelocity += this->rad2deg(pum::dotpro(rperp, impulse)) * this->getInvInertia();
}

void RigidBody::applyCentralImpulse(pum::vector2d impulse)
{
	this->velocity = this->velocity + (impulse * this->getInvMass());
}

void RigidBody::applyRotationalImpulse(long double torque)
{
	this->angularvelocity = this->angularvelocity + (torque * this->getInvInertia());
}

void RigidBody::applyForce(pum::vector2d force, pum::vector2d position)
{
	pum::vector2d r = position - this->position;
	this->constantTorque = this->constantTorque + (pum::dotpro(force, r));
	
	this->constforce = this->constforce + force;
}

void RigidBody::applyCentralForce(pum::vector2d force)
{
	this->constforce = this->constforce + force;
}

void RigidBody::applyTorque(long double torque)
{
	this->constantTorque = this->constantTorque + torque;
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