#pragma once
#include "SFML/Graphics.hpp"
#include "RigidCircle.h"
#include "PhysicsEngine.h"

class Ball
{
public:
	sf::CircleShape shape;
	RigidCircle* physicsbody;
	Ball(double x, double y, double radius, double mass, sf::Color = sf::Color(21, 124, 209));
	~Ball();
	void draw(sf::RenderWindow& window);
	void process(double delta);

	Ball() = delete;
};

