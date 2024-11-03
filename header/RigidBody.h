#pragma once
#include "SFML/Graphics.hpp"
#include "pum.h"

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
		Polygon
	};
		double mass;
		double momentOfInertia;
		double coefficientOfStaticFriction;
		double coefficientOfKineticFriction;
		double angularvelocity = 0;
		pum::vector2d centerOfMass;
		pum::vector2d velocity;
		pum::vector2d acceleration;
		pum::vector2d position;

		double kineticfriction = 0.6;
		double staticfriction = 100;
		double angle = 0;
		bool iscolliding = false;


		type bodyType;
		shapetypes shapetype;
		



		RigidBody(double mass,pum::vector2d velocity,pum::vector2d acceleration,type bodytype, double coefficientOfStaticFriction = 0, double coefficientOfKineticFriction = 0);
		
		double getInvMass();
		double getInvInertia();
		virtual void setrotation(double angle);
		virtual void setpositon(pum::vector2d pos);
		virtual void translate(pum::vector2d pos);
		virtual void rotate(double angle);

		double deg2rad(double deg);
		double rad2deg(double rad);

		
		RigidBody() = delete;
};

