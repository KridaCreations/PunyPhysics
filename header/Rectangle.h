#pragma once
#include "SFML/Graphics.hpp"
#include "PhysicsWorld.h"
#include "RigidRectangelShape.h"
#include "RigidBody.h"

class Rectangle
{
public:
	RigidRectangleShape* physicsbody;
	bool control = false;
	sf::ConvexShape boundbox;
	sf::RectangleShape shape;

	// pos,radius,mass,control = false,type = rigid,color = sf::Color(21, 124, 209)
	Rectangle(sf::Vector2f pos, double width, double length, double mass, bool control = false, RigidBody::type bodytype = RigidBody::Rigid, sf::Color color = sf::Color(21, 124, 209));
	~Rectangle();
	void draw(sf::RenderWindow& window);
	void process(double delta);

	Rectangle() = delete;
};

