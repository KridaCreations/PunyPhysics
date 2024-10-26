#include "../header/PhysicsWorld.h"

#include <iostream>

void PhysicsWorld::addbody(RigidBody* body)
{
	this->bodies.insert(body);

}

void PhysicsWorld::removeBody(RigidBody* body)
{
	this->bodies.erase(body);
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
				collisionresult result = this->checkcollisionpolygoncircle(rect->points, circ->position, circ->radius);
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
					//win->draw()
					(*(this->window)).draw(temp);
					this->resolvecollision((*itr1), (*itr2), result, det);

				}
			}
			else if (((*itr1)->shapetype == RigidBody::Circle) && ((*itr2)->shapetype == RigidBody::Rectangle))
			{
				RigidRectangleShape* rect = static_cast<RigidRectangleShape*>(*itr2);
				RigidCircleShape* circ = static_cast<RigidCircleShape*>(*itr1);
				collisionresult result = this->checkcollisionpolygoncircle(rect->points, circ->position, circ->radius);
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
					this->resolvecollision((*itr1), (*itr2), result, det);
				}
			}
			else if (((*itr1)->shapetype == RigidBody::Rectangle) && ((*itr2)->shapetype == RigidBody::Rectangle))
			{
				RigidRectangleShape* first = static_cast<RigidRectangleShape*>(*itr1);
				RigidRectangleShape* second = static_cast<RigidRectangleShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygonpolygon(first->points, second->points);
				if (result.iscollided)
				{
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
						this->resolvecollision((*itr1), (*itr2), result, det);
					}
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
					this->resolvecollision((*itr2), (*itr1), result, det);
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
	std::cout << "normal " << normal.x << " " << normal.y << std::endl;
	

	std::vector<sf::Vector2f>contaclist(2);
	contaclist[0] = contact1;
	contaclist[1] = contact2;
	std::vector<sf::Vector2f>impulselist(2);
	std::vector<sf::Vector2f>ralist(2);
	std::vector<sf::Vector2f>rblist(2);

	for (int i = 0; i < contactcount; i++)
	{
		std::cout << contaclist[i].x << " " << contaclist[i].y << std::endl;

		sf::Vector2f ra = contaclist[i] - a->position;
		sf::Vector2f rb = contaclist[i] - b->position;

		ralist[i] = ra;
		rblist[i] = rb;

		sf::Vector2f raperp = sf::Vector2f(ra.y, -1.0 * ra.x);
		sf::Vector2f rbperp = sf::Vector2f(rb.y, -1.0 * rb.x);

		std::cout << "raperp " << raperp.x << " " << raperp.y << std::endl;
		std::cout << "rbperp " << rbperp.x << " " << rbperp.y << std::endl;

		sf::Vector2f angularlinearvelocitya = this->mult(raperp, a->angularvelocity);
		sf::Vector2f angularlinearvelocityb = this->mult(rbperp, b->angularvelocity);
		std::cout << "angula A " << angularlinearvelocitya.x << " " << angularlinearvelocitya.y << std::endl;
		std::cout << "angula B" << angularlinearvelocityb.x << " " << angularlinearvelocityb.y << std::endl;


		std::cout << "vel a " << a->velocity.x << " " << a->velocity.y << std::endl;
		std::cout << "vel b " << b->velocity.x << " " << b->velocity.y << std::endl;
		sf::Vector2f relativevelocity = (b->velocity + angularlinearvelocityb) - (a->velocity + angularlinearvelocitya);


		float contactvelocitymag = this->dotpro(relativevelocity, normal);

		std::cout << "contactvelmag " << contactvelocitymag << std::endl;
		double raperpdotn = this->dotpro(raperp, normal);
		double rbperpdotn = this->dotpro(rbperp, normal);

		//double denom = a->getInvMass() + b->getInvMass() ;

		double denom = a->getInvMass() + b->getInvMass() +
			(raperpdotn * raperpdotn) * a->getInvInertia() +
			(rbperpdotn * rbperpdotn) * b->getInvInertia();

		double j = -1.0 * (1.0 + e) * contactvelocitymag;
		j = j / denom;
		j = j / (double)contactcount;

		sf::Vector2f impulse = this->mult(normal, j);
		impulselist[i] = impulse;
		/*float raPerpDotN = FlatMath.Dot(raPerp, normal);
		float rbPerpDotN = FlatMath.Dot(rbPerp, normal);

		float denom = bodyA.InvMass + bodyB.InvMass +
			(raPerpDotN * raPerpDotN) * bodyA.InvInertia +
			(rbPerpDotN * rbPerpDotN) * bodyB.InvInertia;

		float j = -(1f + e) * contactVelocityMag;
		j /= denom;
		j /= (float)contactCount;

		FlatVector impulse = j * normal;
		impulseList[i] = impulse;*/

	}

	for (int i = 0; i < contactcount; i++)
	{
		sf::Vector2f impulse = impulselist[i];
		sf::Vector2f ra = ralist[i];
		sf::Vector2f rb = rblist[i];
		sf::Vector2f achange = this->mult(impulse, a->getInvMass());
		double aangularchange = this->crosspro(ra, impulse) * a->getInvInertia();
		std::cout << "achange " << achange.x << " " << achange.y << std::endl;
		std::cout << "aangularchange " << aangularchange << std::endl;
		//std::cout
		a->velocity -= this->mult(impulse, a->getInvMass());
		//a->angularvelocity += this->crosspro(ra, impulse) * a->getInvInertia();

		sf::Vector2f bchange = this->mult(impulse, b->getInvMass());
		double bangularchange = this->crosspro(rb, impulse) * b->getInvInertia();
		std::cout << "bchange " << bchange.x << " " << bchange.y << std::endl;
		std::cout << "bangularchange " << bangularchange << std::endl;
		b->velocity +=  this->mult(impulse, b->getInvMass());
		//b->angularvelocity += this->crosspro(rb, impulse) * b->getInvInertia();
	}

	/*for (int i = 0; i < contactCount; i++)
	{
		FlatVector impulse = impulseList[i];
		FlatVector ra = raList[i];
		FlatVector rb = rbList[i];

		bodyA.LinearVelocity += -impulse * bodyA.InvMass;
		bodyA.AngularVelocity += -FlatMath.Cross(ra, impulse) * bodyA.InvInertia;
		bodyB.LinearVelocity += impulse * bodyB.InvMass;
		bodyB.AngularVelocity += FlatMath.Cross(rb, impulse) * bodyB.InvInertia;
	}*/

	/*std::cout << impulse.x << " " << impulse.y << " impulse " << std::endl;
	std::cout << (a->velocity.x) << " " << (a->velocity.y) << " velocity a " << std::endl;
	std::cout << (b->velocity.x) << " " << (b->velocity.y) << " velocity a " << std::endl;
	std::cout << (a->getInvMass()) << " invmass a" << std::endl;
	std::cout << (b->getInvMass()) << " invmass b" << std::endl;*/
	/*sf::Vector2f changea = this->mult(impulse, a->getInvMass());
	sf::Vector2f changeb = this->mult(impulse, b->getInvMass() * -1.0);*/
	/*std::cout << (changea.x) << " " << (changea.y) << std::endl;
	std::cout << (changeb.x) << " " << (changeb.y) << std::endl;*/

	std::cout << "===================" << std::endl;
	/*a->velocity += this->mult(impulse, a->getInvMass());
	b->velocity -= this->mult(impulse, b->getInvMass());*/









}

void PhysicsWorld::calculatemovement(double delta)
{
	
	for (auto it : PhysicsWorld::getInstance()->bodies)
	{

		if (it->bodyType == RigidBody::Static)
		{
			it->velocity = it->velocity + this->mult(it->acceleration, delta);
			it->rotate(it->angularvelocity * delta);
		}
		else
		{
			it->velocity = it->velocity + this->mult(it->acceleration + this->gravity, delta);
			it->rotate(it->angularvelocity * delta);
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

PhysicsWorld::collisionresult PhysicsWorld::checkcollisionpolygoncircle(std::vector<sf::Vector2f>& points, sf::Vector2f center, double radius)
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
		if (abs(circle.second - polygon.first) < abs(circle.first - polygon.second))
			depth = circle.second - polygon.first;
		else
			depth = circle.first - polygon.second;

		if (abs(result.depth) > abs(depth))
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
		
		if (abs(circle.second - polygon.first) < abs(circle.first - polygon.second))
			depth = circle.second - polygon.first;
		else
			depth = circle.first - polygon.second;

		if (abs(result.depth) > abs(depth))
		{
			result.depth = depth;
			result.axis = axis;
		}
	}
	result.iscollided = true;
	return result;

	
}

PhysicsWorld::collisionresult PhysicsWorld::checkcollisionpolygonpolygon(std::vector<sf::Vector2f>& points1, std::vector<sf::Vector2f>& points2)
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
		if (abs(polygon2.second - polygon1.first) < abs(polygon2.first - polygon1.second))
			depth = polygon2.second - polygon1.first;
		else
			depth = polygon2.first - polygon1.second;

		if (abs(result.depth) > abs(depth))
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
		if (abs(polygon2.second - polygon1.first) < abs(polygon2.first - polygon1.second))
			depth = polygon2.second - polygon1.first;
		else
			depth = polygon2.first - polygon1.second;

		if (abs(result.depth) > abs(depth))
		{
			result.depth = depth;
			result.axis = axis;
		}
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
					//contact2 = tempres.projectionpoint;
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
					//contact2 = tempres.projectionpoint;
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
	if (mag < 0.5)
		return true;

	return false;
}

bool PhysicsWorld::nearlyequal(double a, double b)
{
	if (abs(a - b) < 0.5)
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
		a->translate(this->mult(axis, depth));
	}
	else
	{
		a->translate(this->mult(axis,depth/2.0));
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