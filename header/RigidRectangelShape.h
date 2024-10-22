#pragma once
#include "RigidBody.h" 
class RigidRectangleShape : public RigidBody
{
public:
	double width, length;
	sf::RectangleShape shape;
	
	RigidRectangleShape(sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f acceleration, double width, double length, double mass, type bodytype, double coefficientOfStaticFriction, double coefficientOfKineticFriction);
	
	
	RigidRectangleShape() = delete;
};

