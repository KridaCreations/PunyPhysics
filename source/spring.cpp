#include "../header/spring.h"

spring::spring(RigidBody* body1, pum::vector2d pos1, RigidBody* body2, pum::vector2d pos2)
{
	this->body1 = body1;
	this->body1point = pos1;
	this->body2 = body2;
	this->body2point = pos2;
}
