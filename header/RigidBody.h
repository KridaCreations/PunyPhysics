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
		double angularvelocity = 0;
		sf::Vector2f centerOfMass;
		sf::Vector2f velocity;
		sf::Vector2f acceleration;
		sf::Vector2f position;

		double kineticfriction = 0.6;
		double staticfriction = 0.0;
		double angle = 0;
		bool iscolliding = false;


		type bodyType;
		shapetypes shapetype;
		



		RigidBody(double mass,sf::Vector2f velocity,sf::Vector2f acceleration,type bodytype, double coefficientOfStaticFriction = 0, double coefficientOfKineticFriction = 0);
		
		double getInvMass();
		double getInvInertia();
		virtual void setrotation(double angle);
		virtual void setpositon(sf::Vector2f pos);
		virtual void translate(sf::Vector2f pos);
		virtual void rotate(double angle);

		double deg2rad(double deg);
		double rad2deg(double rad);

		
		RigidBody() = delete;
};

