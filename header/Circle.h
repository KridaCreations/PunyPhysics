#pragma once
#include "SFML/Graphics.hpp"
#include "PhysicsWorld.h"
#include "RigidCircleShape.h"
#include "RigidBody.h"

class Circle
{
public:
	RigidCircleShape* physicsbody;

	sf::CircleShape shape;

	bool control = false;
	// pos,radius,mass,control = false,type = rigid,color = sf::Color(21, 124, 209)
	Circle(sf::Vector2f pos, double radius, double mass,bool control = false,RigidBody::type bodytype = RigidBody::Rigid, sf::Color color = sf::Color(21, 124, 209));
	~Circle();
	void draw(sf::RenderWindow& window);
	void process(double delta);

	Circle() = delete;
};

