#pragma once
#include "RigidBody.h" 
class RigidRectangleShape : public RigidBody
{
public:
	double width, length;
	std::vector<pum::vector2d>points = {
		pum::vector2d(0,0),pum::vector2d(0,0),pum::vector2d(0,0),pum::vector2d(0,0)
	};
	

	RigidRectangleShape(pum::vector2d pos, pum::vector2d velocity, pum::vector2d acceleration, double width, double length, double mass, type bodytype = type::Rigid, double coefficientOfStaticFriction = 0, double coefficientOfKineticFriction = 0);

	void setrotation(double angle);
	void setpositon(pum::vector2d pos);
	void translate(pum::vector2d pos);
	void rotate(double angle);
	
	RigidRectangleShape() = delete;
};

