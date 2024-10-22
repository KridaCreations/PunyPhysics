#pragma once
#include "RigidBody.h"
class RigidCircleShape : public RigidBody
{
public:
	double radius;
	sf::CircleShape shape;
	RigidCircleShape(sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f acceleration,double radius,double mass,type bodytype = type::Rigid,double coefficientOfStaticFriction = 0,double coefficientOfKineticFriction = 0);

	RigidCircleShape() = delete;
};

