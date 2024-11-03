#pragma once
#include "SFML/Graphics.hpp"
#include "PhysicsWorld.h"
#include "RigidPolygonShape.h"
#include "RigidBody.h"

class Polygon
{
public:
	RigidPolygonShape* physicsbody;
	bool control = false;
	sf::ConvexShape boundbox;
	sf::ConvexShape shape;

	// pos,radius,mass,control = false,type = rigid,color = sf::Color(21, 124, 209)
	Polygon(sf::Vector2f pos,std::vector<sf::Vector2f>points, double mass, bool control = false, RigidBody::type bodytype = RigidBody::Rigid, sf::Color color = sf::Color(21, 124, 209));
	~Polygon();
	void draw(sf::RenderWindow& window);
	void process(double delta);

	Polygon() = delete;
};

