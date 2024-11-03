#pragma once
#include "RigidBody.h" 
class RigidPolygonShape : public RigidBody
{
public:
	std::vector<pum::vector2d>points;

	std::vector<pum::vector2d>relativePointPosition;

	RigidPolygonShape(pum::vector2d pos, pum::vector2d velocity, pum::vector2d acceleration,std::vector<pum::vector2d>points, double mass, type bodytype = type::Rigid, double coefficientOfStaticFriction = 0, double coefficientOfKineticFriction = 0);

	void setrotation(double angle);
	void setpositon(pum::vector2d pos);
	void translate(pum::vector2d pos);
	void rotate(double angle);

	RigidPolygonShape() = delete;
};

