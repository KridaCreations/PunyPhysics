#pragma once
#include "RigidBody.h" 
class RigidRectangleShape : public RigidBody
{
public:
	double width, length;
	std::vector<sf::Vector2f>points = {
		sf::Vector2f(0,0),sf::Vector2f(0,0),sf::Vector2f(0,0),sf::Vector2f(0,0)
	};
	

	RigidRectangleShape(sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f acceleration, double width, double length, double mass, type bodytype = type::Rigid, double coefficientOfStaticFriction = 0, double coefficientOfKineticFriction = 0);

	void setrotation(double angle);
	void setpositon(sf::Vector2f pos);
	void translate(sf::Vector2f pos);
	void rotate(double angle);
	
	RigidRectangleShape() = delete;
};

