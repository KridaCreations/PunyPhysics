#pragma once
#include "SFML/Graphics.hpp"
#include "RigidCircle.h"
#include "PhysicsEngine.h"

class Rod
{
public:
	//sf::CircleShape shape;
	std::vector< sf::CircleShape>shapes;
	std::vector<RigidCircle*>balls;
	int count = 0;
	double degrees = 45;
	int len, radius,x,y;
	Rod(double x, double y, double len, double radius);
	~Rod();
	void draw(sf::RenderWindow& window);
	void process(double delta);

	Rod() = delete;
};

