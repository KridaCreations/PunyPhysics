#pragma once
#include "SFML/Graphics.hpp"

class RigidBody
{
public:
	enum type
	{
		Static,
		Rigid,
		Kinetic
	};
		
	enum shapetypes
	{
		Circle,
		Rectangle,
	};
		double mass;
		double momentOfInertia;
		double coefficientOfStaticFriction;
		double coefficientOfKineticFriction;
		sf::Vector2f centerOfMass;
		sf::Vector2f velocity;
		sf::Vector2f acceleration;

		type bodyType;
		shapetypes shapetype;

		RigidBody(double mass,sf::Vector2f velocity,sf::Vector2f acceleration,type bodytype, double coefficientOfStaticFriction = 0, double coefficientOfKineticFriction = 0);

		RigidBody() = delete;
		//RigidBody(double mass, type bodytype, double coefficientOfStaticFriction, double coefficientOfKineticFriction);
};

