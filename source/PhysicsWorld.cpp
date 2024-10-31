#include "../header/PhysicsWorld.h"

#include <iostream>

void PhysicsWorld::addbody(RigidBody* body)
{
	//this->bodies.insert(body);
	this->bodies.push_back(body);

}

void PhysicsWorld::removeBody(RigidBody* body)
{
	//this->bodies.erase(body);
}

void PhysicsWorld::process(double delta)
{
	framecount++;
	int maxiterations = 8;
	delta = delta / (double)maxiterations;
	for (int i = 0; i < maxiterations; i++)
	{
		calculatemovement(delta);
		collisionDetection();
	}
}

void PhysicsWorld::collisionDetection()
{
	for (auto itr = begin(bodies); itr != bodies.end(); itr++)
	{
		(*itr)->iscolliding = false;
	}
	for (auto itr1 = begin(bodies);itr1 != bodies.end(); itr1++)
	{
		for (auto itr2 = next(itr1); itr2 != bodies.end(); itr2++ )
		{
			if (((*itr1)->bodyType == RigidBody::Static) && ((*itr2)->bodyType == RigidBody::Static))
			{
				continue;
			}

			//checking collision
			if (((*itr1)->shapetype == RigidBody::Rectangle) && ((*itr2)->shapetype == RigidBody::Circle))
			{
				RigidRectangleShape* rect = static_cast<RigidRectangleShape*>(*itr1);
				RigidCircleShape* circ = static_cast<RigidCircleShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygoncircle(rect->position,rect->points, circ->position, circ->radius);
				if (result.iscollided)
				{
					rect->iscolliding = true;
					circ->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);
					contactdetail det = this->findcontactdetailpolygoncircle(rect->points,circ->position,circ->radius);
					
					//drawing collision point
					sf::CircleShape temp;
					temp.setFillColor(sf::Color::Red);
					temp.setRadius(4);
					temp.setOrigin(2,2);
					temp.setPosition(det.contactpoint1);
					
					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);

				}
			}
			else if (((*itr1)->shapetype == RigidBody::Circle) && ((*itr2)->shapetype == RigidBody::Rectangle))
			{
				RigidRectangleShape* rect = static_cast<RigidRectangleShape*>(*itr2);
				RigidCircleShape* circ = static_cast<RigidCircleShape*>(*itr1);
				collisionresult result = this->checkcollisionpolygoncircle(rect->position,rect->points, circ->position, circ->radius);
				if (result.iscollided)
				{
					
					rect->iscolliding = true;
					circ->iscolliding = true;
					this->separateBodies((*itr2), (*itr1), result.axis, result.depth);

					contactdetail det = this->findcontactdetailpolygoncircle(rect->points, circ->position, circ->radius);
					
					//drawing collision point
					sf::CircleShape temp;
					temp.setFillColor(sf::Color::Red);
					temp.setRadius(4);
					temp.setOrigin(2,2);
					temp.setPosition(det.contactpoint1);
					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr2), (*itr1), result, det);
				}
			}
			else if (((*itr1)->shapetype == RigidBody::Rectangle) && ((*itr2)->shapetype == RigidBody::Rectangle))
			{
				RigidRectangleShape* first = static_cast<RigidRectangleShape*>(*itr1);
				RigidRectangleShape* second = static_cast<RigidRectangleShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygonpolygon(first->position,first->points,second->position, second->points);
				if (result.iscollided)
				{
					//std::cout << __LINE__ << std::endl;
					first->iscolliding = true;
					second->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);

					contactdetail det = this->findcontactdetailpolygonpolygon(first->points,second->points);

					
					//drawing collision point
					sf::CircleShape temp1;
					temp1.setFillColor(sf::Color::Red);
					temp1.setRadius(4);
					temp1.setOrigin(2, 2);
					temp1.setPosition(det.contactpoint1);
					(*(this->window)).draw(temp1);
					if (det.contactpointcount == 2)
					{
						sf::CircleShape temp2;
						temp2.setFillColor(sf::Color::Red);
						temp2.setRadius(4);
						temp2.setOrigin(2, 2);
						temp2.setPosition(det.contactpoint2);
						(*(this->window)).draw(temp2);
					}
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);

				}
			}
			else if (((*itr1)->shapetype == RigidBody::Circle) && ((*itr2)->shapetype == RigidBody::Circle))
			{
				RigidCircleShape* first = static_cast<RigidCircleShape*>(*itr1);
				RigidCircleShape* second = static_cast<RigidCircleShape*>(*itr2);
				collisionresult result = this->checkcollisioncirclecircle(first->position, first->radius, second->position, second->radius);
				if (result.iscollided)
				{
					first->iscolliding = true;
					second->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);

					contactdetail det = this->findcontactdetailcirclecircle(first->position, first->radius, second->position, second->radius);

					//drawing collision point
					sf::CircleShape temp;
					temp.setFillColor(sf::Color::Red);
					temp.setRadius(4);
					temp.setOrigin(2, 2);
					temp.setPosition(det.contactpoint1);
					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);
				}
			}
		}
	}
}

void PhysicsWorld::resolvecollision(RigidBody* a, RigidBody* b, collisionresult collision, contactdetail contact)
{

	sf::Vector2f normal = collision.axis;
	sf::Vector2f contact1 = contact.contactpoint1;
	sf::Vector2f contact2 = contact.contactpoint2;
	int contactcount = contact.contactpointcount;


	float e = this->coeffofrestitution;

	std::vector<sf::Vector2f>contaclist(2);
	contaclist[0] = contact1;
	contaclist[1] = contact2;
	std::vector<sf::Vector2f>impulselist(2);
	
	std::vector<sf::Vector2f>ralist(2);
	std::vector<sf::Vector2f>rblist(2);

	for (int i = 0; i < contactcount; i++)
	{
		sf::Vector2f ra = contaclist[i] - a->position;
		sf::Vector2f rb = contaclist[i] - b->position;
		
		ralist[i] = ra;
		rblist[i] = rb;

		sf::Vector2f raperp = sf::Vector2f(-1.0 * ra.y, 1.0 * ra.x);
		sf::Vector2f rbperp = sf::Vector2f(-1.0 * rb.y, 1.0 * rb.x);

		sf::Vector2f angularlinearvelocitya = this->mult(raperp, a->deg2rad(a->angularvelocity));
		sf::Vector2f angularlinearvelocityb = this->mult(rbperp, b->deg2rad(b->angularvelocity));
		sf::Vector2f relativevelocity = (b->velocity + angularlinearvelocityb) - (a->velocity + angularlinearvelocitya);

		float contactvelocitymag = this->dotpro(relativevelocity, normal);

		if (contactvelocitymag < 0.0)
		{
			continue;
		}

		double raperpdotn = this->dotpro(raperp, normal);
		double rbperpdotn = this->dotpro(rbperp, normal);

		double denom = a->getInvMass() + b->getInvMass() +
			(raperpdotn * raperpdotn) * a->getInvInertia() +
			(rbperpdotn * rbperpdotn) * b->getInvInertia();

		double j = -1.0 * (1.0 + e) * contactvelocitymag;
		j = j / denom;
		j = j / (double)contactcount;
		sf::Vector2f impulse = this->mult(normal, j);

		impulselist[i] = impulse;
		
	}

	for (int i = 0; i < contactcount; i++)
	{
		sf::Vector2f impulse = impulselist[i];
		sf::Vector2f ra = ralist[i];
		sf::Vector2f rb = rblist[i];
		sf::Vector2f raperp = sf::Vector2f(-1.0 * ra.y,1.0 * ra.x);
		sf::Vector2f rbperp = sf::Vector2f(-1.0 * rb.y,1.0 * rb.x);

		a->velocity -= this->mult(impulse, a->getInvMass());
		a->angularvelocity -= a->rad2deg(this->dotpro(raperp, impulse)) * a->getInvInertia();

		b->velocity +=  this->mult(impulse, b->getInvMass());
		b->angularvelocity += b->rad2deg(this->dotpro(rbperp, impulse)) * b->getInvInertia();
	}
}


void PhysicsWorld::resolvecollisionwithfriciton(RigidBody* a, RigidBody* b, collisionresult collision, contactdetail contact)
{

	sf::Vector2f normal = collision.axis;
	sf::Vector2f contact1 = contact.contactpoint1;
	sf::Vector2f contact2 = contact.contactpoint2;
	int contactcount = contact.contactpointcount;

	float e = this->coeffofrestitution;
	
	std::vector<sf::Vector2f>contaclist(2);
	contaclist[0] = contact1;
	contaclist[1] = contact2;
	std::vector<sf::Vector2f>impulselist(2);
	std::vector<sf::Vector2f>frictionimpulselist(2);
	std::vector<sf::Vector2f>ralist(2);
	std::vector<sf::Vector2f>rblist(2);
	std::vector<double>jlist(2);

	for (int i = 0; i < contactcount; i++)
	{
		sf::Vector2f ra = contaclist[i] - a->position;
		sf::Vector2f rb = contaclist[i] - b->position;

		ralist[i] = ra;
		rblist[i] = rb;

		sf::Vector2f raperp = sf::Vector2f(-1.0 * ra.y, 1.0 * ra.x);
		sf::Vector2f rbperp = sf::Vector2f(-1.0 * rb.y, 1.0 * rb.x);
		sf::Vector2f angularlinearvelocitya = this->mult(raperp, a->deg2rad(a->angularvelocity));
		sf::Vector2f angularlinearvelocityb = this->mult(rbperp, b->deg2rad(b->angularvelocity));
		sf::Vector2f relativevelocity = (b->velocity + angularlinearvelocityb) - (a->velocity + angularlinearvelocitya);

		float contactvelocitymag = this->dotpro(relativevelocity, normal);
		if (contactvelocitymag < 0.0)
		{
			continue;
		}

		double raperpdotn = this->dotpro(raperp, normal);
		double rbperpdotn = this->dotpro(rbperp, normal);

		double denom = a->getInvMass() + b->getInvMass() +
			(raperpdotn * raperpdotn) * a->getInvInertia() +
			(rbperpdotn * rbperpdotn) * b->getInvInertia();

		double j = -1.0 * (1.0 + e) * contactvelocitymag;
		j = j / denom;
		j = j / (double)contactcount;

		jlist[i] = j;

		sf::Vector2f impulse = this->mult(normal, j);
		impulselist[i] = impulse;

	}

	for (int i = 0; i < contactcount; i++)
	{
		sf::Vector2f impulse = impulselist[i];
		sf::Vector2f ra = ralist[i];
		sf::Vector2f rb = rblist[i];

		sf::Vector2f raperp = sf::Vector2f(-1.0 * ra.y, 1.0 * ra.x);
		sf::Vector2f rbperp = sf::Vector2f(-1.0 * rb.y, 1.0 * rb.x);

		a->velocity -= this->mult(impulse, a->getInvMass());
		a->angularvelocity -= a->rad2deg(this->dotpro(raperp, impulse)) * a->getInvInertia();

		b->velocity += this->mult(impulse, b->getInvMass());
		b->angularvelocity += b->rad2deg(this->dotpro(rbperp, impulse)) * b->getInvInertia();
	}


	for (int i = 0; i < contactcount; i++)
	{
		sf::Vector2f ra = contaclist[i] - a->position;
		sf::Vector2f rb = contaclist[i] - b->position;

		ralist[i] = ra;
		rblist[i] = rb;

		sf::Vector2f raperp = sf::Vector2f(-1.0 * ra.y, 1.0 * ra.x);
		sf::Vector2f rbperp = sf::Vector2f(-1.0 * rb.y, 1.0 * rb.x);
		sf::Vector2f angularlinearvelocitya = this->mult(raperp, a->deg2rad(a->angularvelocity));
		sf::Vector2f angularlinearvelocityb = this->mult(rbperp, b->deg2rad(b->angularvelocity));
		sf::Vector2f relativevelocity = (b->velocity + angularlinearvelocityb) - (a->velocity + angularlinearvelocitya);

		float contactvelocitymag = this->dotpro(relativevelocity, normal);
		sf::Vector2f tangent = relativevelocity - this->mult(normal, contactvelocitymag);
		
		if (this->nearlyequal(tangent, sf::Vector2f(0, 0)))
		{
			continue;
		}
		else
		{
			tangent = this->setmag(tangent, 1);
		}

		double raperpdott = this->dotpro(raperp, tangent);
		double rbperpdott = this->dotpro(rbperp, tangent);

		double denom = a->getInvMass() + b->getInvMass() +
			(raperpdott * raperpdott) * a->getInvInertia() +
			(rbperpdott * rbperpdott) * b->getInvInertia();

		double jf = -1.0 * this->dotpro(relativevelocity,tangent);
		jf = jf / denom;
		jf = jf / (double)contactcount;

		sf::Vector2f frictionimpulse;
		double j = jlist[i];

		double sf = std::min(a->staticfriction, b->staticfriction);

		if (std::abs(jf) <= abs(j * sf))
		{
			frictionimpulse = this->mult(tangent,jf);
		}
		else
		{
			double sign = jf / std::abs(jf);
			frictionimpulse = this->mult(tangent, sign * abs(j) * sf);
		}
		frictionimpulselist[i] = frictionimpulse;
	}

	for (int i = 0; i < contactcount; i++)
	{
		sf::Vector2f frictionimpulse = frictionimpulselist[i];
		sf::Vector2f ra = ralist[i];
		sf::Vector2f rb = rblist[i];


		sf::Vector2f raperp = sf::Vector2f(-1.0 * ra.y, 1.0 * ra.x);
		sf::Vector2f rbperp = sf::Vector2f(-1.0 * rb.y, 1.0 * rb.x);

		a->velocity -= this->mult(frictionimpulse, a->getInvMass());
		a->angularvelocity -= a->rad2deg(this->dotpro(raperp, frictionimpulse)) * a->getInvInertia();

		b->velocity += this->mult(frictionimpulse, b->getInvMass());
		b->angularvelocity += b->rad2deg(this->dotpro(rbperp, frictionimpulse)) * b->getInvInertia();

	}

	
}

void PhysicsWorld::calculatemovement(double delta)
{
	
	for (auto it : PhysicsWorld::getInstance()->bodies)
	{

		if (it->bodyType == RigidBody::Static)
		{
			it->velocity = it->velocity + this->mult(it->acceleration, delta);
			if (this->nearlyequal(it->velocity, sf::Vector2f(0, 0)))
			{
				it->velocity = sf::Vector2f(0,0);
			}
		}
		else
		{
			it->velocity = it->velocity + this->mult(it->acceleration + this->gravity, delta);
			if (this->nearlyequal(it->velocity, sf::Vector2f(0, 0)))
			{
				it->velocity = sf::Vector2f(0, 0);
			}
		}
		if (it->shapetype == RigidBody::shapetypes::Circle)
		{
			it->translate(this->mult(it->velocity,delta));
			it->rotate(it->angularvelocity * delta);
		}
		else if (it->shapetype == RigidBody::shapetypes::Rectangle)
		{
			it->translate(this->mult(it->velocity, delta));
			it->rotate(it->angularvelocity * delta);
		}
	}
}


double PhysicsWorld::getmag(sf::Vector2f vec)
{
	double length = std::sqrtf((vec.x * vec.x) + (vec.y * vec.y));
	return length;
}

sf::Vector2f PhysicsWorld::setmag(sf::Vector2f vec, double mag)
{
	double length = std::sqrtf((vec.x * vec.x) + (vec.y * vec.y));
	vec.x = (vec.x * mag) / length;
	vec.y = (vec.y * mag) / length;
	return vec;

}

double PhysicsWorld::dotpro(sf::Vector2f vec1, sf::Vector2f vec2)
{
	double ans = (vec1.x * vec2.x) + (vec1.y * vec2.y);
	return (ans);
}

double PhysicsWorld::crosspro(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

sf::Vector2f PhysicsWorld::mult(sf::Vector2f vec, double factor)
{
	vec.x = vec.x * factor;
	vec.y = vec.y * factor;
	return vec;
}

double PhysicsWorld::getmagsquared(sf::Vector2f a)
{
	return (a.x * a.x) + (a.y * a.y);
}

PhysicsWorld::collisionresult PhysicsWorld::checkcollisionpolygoncircle(sf::Vector2f centerrect ,std::vector<sf::Vector2f>& points, sf::Vector2f center, double radius)
{
	PhysicsWorld::collisionresult result;
	for (int point = 0; point < points.size(); point++)
	{
		sf::Vector2f a = points[point];
		sf::Vector2f b = points[(point + 1) % points.size()];
		sf::Vector2f axis = b - a; //finding the egde
		axis = sf::Vector2f(axis.y, axis.x * -1); //making it perpendicular to actual finding an axis
		axis = this->setmag(axis, 1);
		std::pair<double,double>polygon = this->projectpolygon(axis, points);
		std::pair<double,double>circle = this->projectcircle(axis, center, radius);
		
		if ((polygon.second <= circle.first) || (circle.second <= polygon.first))
		{
			
			result.iscollided = false;
			return result; //gap detected
		}


		double depth;
		depth = std::min((polygon.second - circle.first), (circle.second - polygon.first));
		if (result.depth > depth)
		{
			result.depth = depth;
			result.axis = axis;
		}
		
		axis = center - a;
		axis = this->setmag(axis, 1);
		polygon = this->projectpolygon(axis, points);
		circle = this->projectcircle(axis, center, radius);

		if ((polygon.second <= circle.first) || (circle.second <= polygon.first))
		{
			result.iscollided = false;
			return result; //gap detected
		}
		
		depth = std::min((polygon.second - circle.first), (circle.second - polygon.first));
		if (result.depth > depth)
		{
			result.depth = depth;
			result.axis = axis;
		}

	}

	sf::Vector2f direction = centerrect - center;
	if (this->dotpro(direction, result.axis) < 0.0)
	{
		result.axis = this->mult(result.axis, -1);
	}
	result.iscollided = true;
	return result;

	
}

PhysicsWorld::collisionresult PhysicsWorld::checkcollisionpolygonpolygon(sf::Vector2f center1,std::vector<sf::Vector2f>& points1,sf::Vector2f center2, std::vector<sf::Vector2f>& points2)
{
	PhysicsWorld::collisionresult result;
	for (int point = 0; point < points1.size(); point++)
	{
		sf::Vector2f a = points1[point];
		sf::Vector2f b = points1[(point + 1) % points1.size()];
		sf::Vector2f axis = this->setmag(b - a, 1);
		axis = sf::Vector2f(axis.y, axis.x * -1);

		std::pair<double, double>polygon1 = this->projectpolygon(axis, points1);
		std::pair<double, double>polygon2 = this->projectpolygon(axis, points2);
		
		if ((polygon1.second <= polygon2.first) || (polygon2.second <= polygon1.first))
		{
			result.iscollided = false;
			return result; //gap detected
		}
		double depth;
		depth = std::min(polygon1.second - polygon2.first, polygon2.second - polygon1.first);
		if (depth < result.depth)
		{
			result.depth = depth;
			result.axis = axis;
		}
	}
	for (int point = 0; point < points2.size(); point++)
	{
		sf::Vector2f a = points2[point];
		sf::Vector2f b = points2[(point + 1) % points2.size()];
		sf::Vector2f axis = this->setmag(b - a, 1);
		axis = sf::Vector2f(axis.y, axis.x * -1);

		std::pair<double, double>polygon1 = this->projectpolygon(axis, points1);
		std::pair<double, double>polygon2 = this->projectpolygon(axis, points2);
		
		if ((polygon1.second <= polygon2.first) || (polygon2.second <= polygon1.first))
		{
			result.iscollided = false;
			return result; //gap detected
		}
		double depth;
		depth = std::min(polygon1.second - polygon2.first, polygon2.second - polygon1.first);
		if (depth < result.depth)
		{
			result.depth = depth;
			result.axis = this->mult(axis,1);
		}
	}

	sf::Vector2f direction = center1 - center2;
	if (this->dotpro(direction, result.axis) < 0.0)
	{
		result.axis = this->mult(result.axis, -1);
	}
	result.iscollided = true;
	return result;
}

PhysicsWorld::collisionresult PhysicsWorld::checkcollisioncirclecircle(sf::Vector2f center1, double radius1, sf::Vector2f center2, double radius2)
{
	PhysicsWorld::collisionresult result;
	sf::Vector2f axis = center1 - center2;
	double distance = this->getmag(axis);
	axis = this->setmag(axis, 1.0);
	double radiisum = radius1 + radius2;
	result.iscollided = false;
	if (distance < (radiisum))
	{
		result.iscollided = true;
		this->setmag(axis, 1);
		result.depth = (radiisum - distance);
		result.axis = axis;

		sf::Vector2f direction = center1 - center2;
		if (this->dotpro(direction, result.axis) < 0.0)
		{
			result.axis = this->mult(result.axis, -1);
		}
		result.iscollided = true;
	}
	return result;
}

PhysicsWorld::contactdetail PhysicsWorld::findcontactdetailpolygonpolygon(std::vector<sf::Vector2f> points1, std::vector<sf::Vector2f> points2)
{
	contactdetail result;
	result.contactpointcount = 0;
	double mindis = LLONG_MAX;


	for (int point1 = 0; point1 < points1.size(); point1++)
	{
		sf::Vector2f p = points1[point1];
		for (int point2 = 0; point2 < points2.size(); point2++)
		{
			sf::Vector2f a = points2[point2];
			sf::Vector2f b = points2[(point2 + 1) % points2.size()];

			pointlineprojectresult tempres = this->distanceFromLinesegment(a, b, p);
			if (nearlyequal(tempres.distance, mindis))
			{
				if (nearlyequal(tempres.projectionpoint, result.contactpoint1) == false)
				{
					result.contactpoint2 = tempres.projectionpoint;
					result.contactpointcount = 2;
				}
			}
			else if (mindis > tempres.distance)
			{
				result.contactpoint1 = tempres.projectionpoint;
				result.contactpointcount = 1;
				mindis = tempres.distance;
			}

		}
	}
	for (int point2 = 0; point2 < points2.size(); point2++)
	{
		sf::Vector2f p = points2[point2];
		for (int point1 = 0; point1 < points1.size(); point1++)
		{
			sf::Vector2f a = points1[point1];
			sf::Vector2f b = points1[(point1 + 1) % points1.size()];

			pointlineprojectresult tempres = this->distanceFromLinesegment(a, b, p);
			if (nearlyequal(tempres.distance, mindis))
			{
				if (nearlyequal(tempres.projectionpoint, result.contactpoint1) == false)
				{
					result.contactpoint2 = tempres.projectionpoint;
					result.contactpointcount = 2;
				}
			}
			else if (mindis > tempres.distance)
			{
				result.contactpoint1 = tempres.projectionpoint;
				result.contactpointcount = 1;
				mindis = tempres.distance;
			}

		}
	}
	return result;
}

PhysicsWorld::contactdetail PhysicsWorld::findcontactdetailpolygoncircle(std::vector<sf::Vector2f> points, sf::Vector2f center, double radius)
{
	contactdetail result;	
	result.contactpointcount = 0;
	double mindis = LLONG_MAX;

	sf::Vector2f p = center;
	for (int point = 0; point < points.size(); point++)
	{
		sf::Vector2f a = points[point];
		sf::Vector2f b = points[(point + 1) % points.size()];

		pointlineprojectresult tempres = this->distanceFromLinesegment(a, b, p);
		if(mindis > tempres.distance)
		{
			result.contactpoint1 = tempres.projectionpoint;
			result.contactpointcount = 1;
			mindis = tempres.distance;
		}
	}
	return result;
}

PhysicsWorld::contactdetail PhysicsWorld::findcontactdetailcirclecircle(sf::Vector2f center1, double radius1, sf::Vector2f center2, double radius2)
{
	contactdetail result;
	result.contactpointcount = 0;

	sf::Vector2f axis = center1 - center2;
	axis = this->setmag(axis, 1);
	sf::Vector2f p = center2 + this->mult(axis, radius2);

	result.contactpoint1 = p;
	result.contactpointcount = 1;
	return result;

}


bool PhysicsWorld::nearlyequal(sf::Vector2f a, sf::Vector2f b)
{
	sf::Vector2f axis = a - b;
	double mag = this->getmag(axis);
	if(mag == 0)
	if (mag < 0.00005)
		return true;

	return false;
}

bool PhysicsWorld::nearlyequal(double a, double b)
{
	if (abs(a - b) < 0.00005)
		return true;

	return false;
}

std::pair<double, double> PhysicsWorld::projectcircle(sf::Vector2f line, sf::Vector2f point, double radius)
{
	sf::Vector2f linenormal = this->setmag(line, 1.0);
	sf::Vector2f left = point - this->mult(linenormal, radius);
	sf::Vector2f right = point + this->mult(linenormal, radius);
	double dotleft = this->dotpro(line, left);
	double dotright = this->dotpro(line, right);
	if (dotleft < dotright)
		return std::pair<double, double>(dotleft, dotright);
	return std::pair<double, double>(dotright, dotleft);
}

std::pair<double, double> PhysicsWorld::projectpolygon(sf::Vector2f line, std::vector<sf::Vector2f>& points)
{
	double minim = LLONG_MAX;
	double maxim = LLONG_MIN;
	for (int point = 0; point < points.size(); point++)
	{
		sf::Vector2f p = points[point];
		double val = this->dotpro(line, p);
		minim = std::min(minim, val);
		maxim = std::max(maxim, val);
	}
	return std::pair<double, double>(minim, maxim);
}

void PhysicsWorld::separateBodies(RigidBody* a, RigidBody* b,sf::Vector2f axis,double depth)
{
	if (a->bodyType == RigidBody::Static)
	{
		b->translate(this->mult(axis, depth * (-1.0)));
	}
	else if (b->bodyType == RigidBody::Static)
	{
		a->translate(this->mult(axis, depth * 1.0));
	}
	else
	{
		a->translate(this->mult(axis, (depth * 1.0) / 2.0));
		b->translate(this->mult(axis, (depth * -1.0) / 2.0));
	}
}


PhysicsWorld::pointlineprojectresult PhysicsWorld::distanceFromLinesegment(sf::Vector2f a, sf::Vector2f b, sf::Vector2f p)
{

	PhysicsWorld::pointlineprojectresult result;

	sf::Vector2f ab = b - a;
	sf::Vector2f ap = p - a;
	double dot = this->dotpro(ab, ap);
	
	double d = dot / this->getmagsquared(ab);
	sf::Vector2f cp;
	if (d <= 0.0)
	{
		cp = a;
	}
	else if (d >= 1.0)
	{
		cp = b;
	}
	else
	{
		cp = a + this->mult(ab, d);
	}

	double dis = this->getmag(cp - p);
	result.distance = dis;
	result.projectionpoint = cp;
	return result;

}