#pragma once
#include "RigidBody.h"
#include <vector>
#include "pum.h"
//#include <pa>

class RigidCircleShape : public RigidBody
{
public:
	double radius = 0;
	

	RigidCircleShape(pum::vector2d pos, pum::vector2d velocity, pum::vector2d acceleration,double radius,double mass,type bodytype = type::Rigid,double coefficientOfStaticFriction = 0,double coefficientOfKineticFriction = 0);
	
	void setrotation(double angle);
	void setpositon(pum::vector2d pos);
	void translate(pum::vector2d pos);
	void rotate(double angle);

	RigidCircleShape() = delete;
};

