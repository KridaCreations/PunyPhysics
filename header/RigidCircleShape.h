#pragma once
#include "RigidBody.h"
#include <vector>
//#include <pa>

class RigidCircleShape : public RigidBody
{
public:
	double radius = 0;
	

	RigidCircleShape(sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f acceleration,double radius,double mass,type bodytype = type::Rigid,double coefficientOfStaticFriction = 0,double coefficientOfKineticFriction = 0);
	
	void setrotation(double angle);
	void setpositon(sf::Vector2f pos);
	void translate(sf::Vector2f pos);
	void rotate(double angle);

	RigidCircleShape() = delete;
};

