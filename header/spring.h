
#pragma once
#include "RigidBody.h" 
class spring
{
public:
	//std::vector < std::pair<pum::vector2d, RigidBody*>>bodies;
public :
	RigidBody* body1 = nullptr;
	RigidBody* body2 = nullptr;
	pum::vector2d body1point = pum::vector2d();
	pum::vector2d body2point = pum::vector2d();

	double springconstant = 000;
	double maxlen = 1;
	double minlen = 1;
	double length = 1;
	double coefficientofdamping = 5000;

	spring(RigidBody* body1, pum::vector2d pos1, RigidBody* body2, pum::vector2d pos2);

	spring() = delete;
};

