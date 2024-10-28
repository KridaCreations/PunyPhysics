#pragma once

#include<set>
#include<vector>

#include "RigidCircleShape.h"
#include "RigidRectangelShape.h"
#include "RigidBody.h"
#include "imgui-SFML.h"
#include "imgui.h"

class PhysicsWorld
{
private:
	static PhysicsWorld* instancePtr;

public:

	struct collisionresult {
		bool iscollided = false;
		double depth = LLONG_MAX;
		sf::Vector2f axis;
	} ;
	struct pointlineprojectresult {
		sf::Vector2f projectionpoint = sf::Vector2f(0,0);
		double distance = 0;
	};
	struct contactdetail {
		sf::Vector2f contactpoint1 = sf::Vector2f(0,0);
		sf::Vector2f contactpoint2 = sf::Vector2f(0,0);
		int contactpointcount = 0;
	};



	sf::Vector2f setmag(sf::Vector2f vec, double mag);
	double dotpro(sf::Vector2f vec1, sf::Vector2f vec2);
	double crosspro(sf::Vector2f vec1, sf::Vector2f vec2);
	sf::Vector2f mult(sf::Vector2f vec, double factor);
	double getmag(sf::Vector2f vect);
	

	void calculatemovement(double delta);
	void collisionDetection();


	void addbody(RigidBody* body);
	void removeBody(RigidBody* body);
	void process(double delta);

	int framecount = 0;
	//std::set<RigidBody*>bodies;
	std::vector<RigidBody*>bodies;
	sf::Vector2f gravity = sf::Vector2f(0.0, 0.0);
	double coeffofrestitution = 0.8;


	sf::RenderWindow* window;


private:
	PhysicsWorld()
	{
		
	};
public:
	double getmagsquared(sf::Vector2f a);
	std::pair<double, double> projectcircle(sf::Vector2f line, sf::Vector2f point,double radius);
	std::pair<double, double> projectpolygon(sf::Vector2f line, std::vector<sf::Vector2f>& points);
	void separateBodies(RigidBody* a, RigidBody* b, sf::Vector2f axis, double depth);
	pointlineprojectresult distanceFromLinesegment(sf::Vector2f a, sf::Vector2f b, sf::Vector2f point);
	
	PhysicsWorld::collisionresult checkcollisionpolygoncircle(sf::Vector2f centerrect,std::vector<sf::Vector2f>& points, sf::Vector2f center, double radius);
	PhysicsWorld::collisionresult checkcollisionpolygonpolygon(sf::Vector2f center1, std::vector<sf::Vector2f>& points1, sf::Vector2f center2, std::vector<sf::Vector2f>& points2);
	PhysicsWorld::collisionresult checkcollisioncirclecircle(sf::Vector2f center1, double radius1, sf::Vector2f center2, double radius2);

	PhysicsWorld::contactdetail findcontactdetailpolygonpolygon(std::vector<sf::Vector2f>points1, std::vector<sf::Vector2f>points2);
	PhysicsWorld::contactdetail findcontactdetailpolygoncircle(std::vector<sf::Vector2f>points, sf::Vector2f center, double radius);
	PhysicsWorld::contactdetail findcontactdetailcirclecircle(sf::Vector2f center1,double radius1,sf::Vector2f center2,double radius2);
	
	void resolvecollision(RigidBody* a, RigidBody* b, collisionresult, contactdetail);

	bool nearlyequal(sf::Vector2f a, sf::Vector2f b);
	bool nearlyequal(double a, double b);
public:
	static PhysicsWorld* getInstance() {
		if (instancePtr == nullptr) {
			instancePtr = new PhysicsWorld();
		}
		return instancePtr;
	}
};

