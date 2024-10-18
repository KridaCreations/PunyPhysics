#pragma once
#include "SFML/Graphics.hpp"
#include "RigidCircle.h"
#include "PhysicsEngine.h"

class staticball
{
public:
	sf::CircleShape shape;
	RigidCircle* physicsbody;
	staticball(double x, double y, double radius, sf::Color = sf::Color(21, 124, 209));
	~staticball();
	void draw(sf::RenderWindow& window);
	void process(double delta);

	staticball() = delete;
};

