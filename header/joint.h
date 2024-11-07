
#pragma once
#include "RigidBody.h" 
class joint
{
public:
	std::vector < std::pair<pum::vector2d, RigidBody*>>bodies;

	pum::vector2d pinposition;

	double radius;

	joint(double radius, bool pinned, pum::vector2d pinposition = pum::vector2d(0,0));
	
	void addbodies(RigidBody* body, pum::vector2d pos);

	joint() = delete;
};

