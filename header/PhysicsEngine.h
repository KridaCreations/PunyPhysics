#pragma once
#include "RigidCircle.h"
#include "BoundBox.h"
#include <set>
#include <vector>

class PhysicsEngine
{
	private:
		static PhysicsEngine* instancePtr;
		


	public:
		int addObject(RigidCircle* obj);
		void removeObject(int index);
		void testingengine();
		void process(double delta);
		sf::Vector2f setmag(sf::Vector2f vec, double mag);
		double dotpro(sf::Vector2f vec1, sf::Vector2f vec2);
		sf::Vector2f mult(sf::Vector2f vec, double factor);
		double getmag(sf::Vector2f vect);
		void solveCollisionDirect();
		void solveCollisionCumulative();
		void calculatemovement(double delta);
		

		sf::Vector2f gravity = sf::Vector2f(0.0, 0.0);
		BoundBox* bh = new BoundBox(200, 200, 50, 50);
		std::set<RigidCircle*>staticobjects;
		std::set<RigidCircle*>rigidobjects;
		std::vector<RigidCircle*>indexes;
		std::vector<int>unusedindex;
		int maxindex = 0;
		double coeffofrestitution = 1;
		




	private:
		PhysicsEngine()
		{

		};

	public:
		static PhysicsEngine* getInstance() {
			if (instancePtr == nullptr) {
					instancePtr = new PhysicsEngine();
			}
			return instancePtr;
		}
};

