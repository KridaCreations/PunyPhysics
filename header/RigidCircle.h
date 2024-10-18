#pragma once
#include "SFML/Graphics.hpp"

class RigidCircle
{
public:
	double mass;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	sf::Vector2f pos;

	sf::Vector2f prevpos;
	sf::Vector2f prevacceleration;
	sf::Vector2f prevvelocity;

	sf::Vector2f veladd;
	sf::Vector2f acceladd;
	sf::Vector2f posadd;

	enum bodytype
	{
		_static,_rigid
	};

	bodytype type;

	double radius;
	RigidCircle(sf::Vector2f pos,double mass,double radius, bodytype type, sf::Vector2f velocity = sf::Vector2f(0,0), sf::Vector2f acceleration = sf::Vector2f(0, 0));


	RigidCircle() = delete;
};

