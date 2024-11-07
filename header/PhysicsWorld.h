#pragma once

#include<set>
#include<vector>

#include "RigidCircleShape.h"
#include "RigidRectangelShape.h"
#include "RigidBody.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "pum.h"
#include "joint.h"
#include <math.h>

class PhysicsWorld
{
private:
	static PhysicsWorld* instancePtr;

public:

	struct collisionresult {
		bool iscollided = false;
		double depth = LLONG_MAX;
		pum::vector2d axis;
	} ;
	
	struct contactdetail {
		pum::vector2d contactpoint1 = pum::vector2d(0,0);
		pum::vector2d contactpoint2 = pum::vector2d(0,0);
		int contactpointcount = 0;
	};


	void resolvecollisionwithfriciton(RigidBody* a, RigidBody* b, collisionresult collision, contactdetail contact);

	void calculatemovement(double delta);
	void collisionDetection();
	void constraintsolving();
	void solvejoints(joint* nail);


	void addbody(RigidBody* body);
	void removeBody(RigidBody* body);
	void process(double delta);

	int framecount = 0;
	std::vector<RigidBody*>bodies;
	std::vector<joint*>joints;
	pum::vector2d gravity = pum::vector2d(0.0, 0.0);
	double coeffofrestitution = 0.3;
	sf::RenderWindow* window;


private:
	PhysicsWorld()
	{
		
	};
public:
	void separateBodies(RigidBody* a, RigidBody* b, pum::vector2d axis, double depth);
	
	PhysicsWorld::collisionresult checkcollisionpolygoncircle(pum::vector2d centerrect,std::vector<pum::vector2d>& points, pum::vector2d center, double radius);
	PhysicsWorld::collisionresult checkcollisionpolygonpolygon(pum::vector2d center1, std::vector<pum::vector2d>& points1, pum::vector2d center2, std::vector<pum::vector2d>& points2);
	PhysicsWorld::collisionresult checkcollisioncirclecircle(pum::vector2d center1, double radius1, pum::vector2d center2, double radius2);

	PhysicsWorld::contactdetail findcontactdetailpolygonpolygon(std::vector<pum::vector2d>points1, std::vector<pum::vector2d>points2);
	PhysicsWorld::contactdetail findcontactdetailpolygoncircle(std::vector<pum::vector2d>points, pum::vector2d center, double radius);
	PhysicsWorld::contactdetail findcontactdetailcirclecircle(pum::vector2d center1,double radius1,pum::vector2d center2,double radius2);

	bool nearlyequal(pum::vector2d a, pum::vector2d b);
	bool nearlyequal(double a, double b);
public:
	static PhysicsWorld* getInstance() {
		if (instancePtr == nullptr) {
			instancePtr = new PhysicsWorld();
		}
		return instancePtr;
	}
};

