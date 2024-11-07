#include "../header/joint.h"

joint::joint(double radius, bool pinned, pum::vector2d pinposition )
{
	this->radius = radius;
	

}

void joint::addbodies(RigidBody* body, pum::vector2d pos)
{
	this->bodies.push_back({ pos,body });
}
