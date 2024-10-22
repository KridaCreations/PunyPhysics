#pragma once

#include<set>
#include<vector>

#include "RigidCircleShape.h"
#include "RigidRectangelShape.h"
#include "RigidBody.h"


class PhysicsWorld
{
private:
	static PhysicsWorld* instancePtr;

public:
	sf::Vector2f setmag(sf::Vector2f vec, double mag);
	double dotpro(sf::Vector2f vec1, sf::Vector2f vec2);
	sf::Vector2f mult(sf::Vector2f vec, double factor);
	double getmag(sf::Vector2f vect);
	void solveCollisionDirect();
	void solveCollisionCumulative();
	void calculatemovement(double delta);
	int framecount = 0;
	std::set<RigidBody*>bodies;

	void addbody(RigidBody* body);
	void removeBody(RigidBody* body);
	void process(double delta);
	sf::Vector2f gravity = sf::Vector2f(0.0, 0.0);

	


private:
	PhysicsWorld()
	{
		
	};

public:
	static PhysicsWorld* getInstance() {
		if (instancePtr == nullptr) {
			instancePtr = new PhysicsWorld();
		}
		return instancePtr;
	}
};

