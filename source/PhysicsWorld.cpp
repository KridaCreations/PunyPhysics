#include "../header/PhysicsWorld.h"

#include <iostream>
#include "../header/RigidPolygonShape.h"

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
	for (auto itr1 = begin(bodies); itr1 != bodies.end(); itr1++)
	{
		for (auto itr2 = next(itr1); itr2 != bodies.end(); itr2++)
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
				collisionresult result = this->checkcollisionpolygoncircle(rect->position, rect->points, circ->position, circ->radius);
				if (result.iscollided)
				{
					rect->iscolliding = true;
					circ->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);
					contactdetail det = this->findcontactdetailpolygoncircle(rect->points, circ->position, circ->radius);

					//drawing collision point
					sf::CircleShape temp;
					temp.setFillColor(sf::Color::Red);
					temp.setRadius(4);
					temp.setOrigin(2, 2);
					temp.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));

					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);

				}
			}
			else if (((*itr1)->shapetype == RigidBody::Circle) && ((*itr2)->shapetype == RigidBody::Rectangle))
			{
				RigidRectangleShape* rect = static_cast<RigidRectangleShape*>(*itr2);
				RigidCircleShape* circ = static_cast<RigidCircleShape*>(*itr1);
				collisionresult result = this->checkcollisionpolygoncircle(rect->position, rect->points, circ->position, circ->radius);
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
					temp.setOrigin(2, 2);
					temp.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr2), (*itr1), result, det);
				}
			}
			else if (((*itr1)->shapetype == RigidBody::Rectangle) && ((*itr2)->shapetype == RigidBody::Rectangle))
			{
				RigidRectangleShape* first = static_cast<RigidRectangleShape*>(*itr1);
				RigidRectangleShape* second = static_cast<RigidRectangleShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygonpolygon(first->position, first->points, second->position, second->points);
				if (result.iscollided)
				{
					first->iscolliding = true;
					second->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);

					contactdetail det = this->findcontactdetailpolygonpolygon(first->points, second->points);


					//drawing collision point
					sf::CircleShape temp1;
					temp1.setFillColor(sf::Color::Red);
					temp1.setRadius(4);
					temp1.setOrigin(2, 2);
					temp1.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
					(*(this->window)).draw(temp1);
					if (det.contactpointcount == 2)
					{
						sf::CircleShape temp2;
						temp2.setFillColor(sf::Color::Red);
						temp2.setRadius(4);
						temp2.setOrigin(2, 2);
						temp2.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
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

					sf::CircleShape temp;
					temp.setFillColor(sf::Color::Red);
					temp.setRadius(4);
					temp.setOrigin(2, 2);
					temp.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);
				}
			}
			else if (((*itr1)->shapetype == RigidBody::Polygon) && ((*itr2)->shapetype == RigidBody::Circle))
			{
				RigidPolygonShape* rect = static_cast<RigidPolygonShape*>(*itr1);
				RigidCircleShape* circ = static_cast<RigidCircleShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygoncircle(rect->position, rect->points, circ->position, circ->radius);
				if (result.iscollided)
				{
					rect->iscolliding = true;
					circ->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);
					contactdetail det = this->findcontactdetailpolygoncircle(rect->points, circ->position, circ->radius);

					//drawing collision point
					sf::CircleShape temp;
					temp.setFillColor(sf::Color::Red);
					temp.setRadius(4);
					temp.setOrigin(2, 2);
					temp.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));

					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);

				}
				}
			else if (((*itr1)->shapetype == RigidBody::Circle) && ((*itr2)->shapetype == RigidBody::Polygon))
			{
				RigidPolygonShape* rect = static_cast<RigidPolygonShape*>(*itr2);
				RigidCircleShape* circ = static_cast<RigidCircleShape*>(*itr1);
				collisionresult result = this->checkcollisionpolygoncircle(rect->position, rect->points, circ->position, circ->radius);
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
					temp.setOrigin(2, 2);
					temp.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
					(*(this->window)).draw(temp);
					this->resolvecollisionwithfriciton((*itr2), (*itr1), result, det);
				}
				}
			else if (((*itr1)->shapetype == RigidBody::Polygon) && ((*itr2)->shapetype == RigidBody::Polygon))
			{
				RigidPolygonShape* first = static_cast<RigidPolygonShape*>(*itr1);
				RigidPolygonShape* second = static_cast<RigidPolygonShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygonpolygon(first->position, first->points, second->position, second->points);
				if (result.iscollided)
				{
					first->iscolliding = true;
					second->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);

					contactdetail det = this->findcontactdetailpolygonpolygon(first->points, second->points);


					//drawing collision point
					sf::CircleShape temp1;
					temp1.setFillColor(sf::Color::Red);
					temp1.setRadius(4);
					temp1.setOrigin(2, 2);
					temp1.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
					(*(this->window)).draw(temp1);
					if (det.contactpointcount == 2)
					{
						sf::CircleShape temp2;
						temp2.setFillColor(sf::Color::Red);
						temp2.setRadius(4);
						temp2.setOrigin(2, 2);
						temp2.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
						(*(this->window)).draw(temp2);
					}
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);

				}
			}
			else if (((*itr1)->shapetype == RigidBody::Rectangle) && ((*itr2)->shapetype == RigidBody::Polygon))
			{
				RigidRectangleShape* first = static_cast<RigidRectangleShape*>(*itr1);
				RigidPolygonShape* second = static_cast<RigidPolygonShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygonpolygon(first->position, first->points, second->position, second->points);
				if (result.iscollided)
				{
					first->iscolliding = true;
					second->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);

					contactdetail det = this->findcontactdetailpolygonpolygon(first->points, second->points);


					//drawing collision point
					sf::CircleShape temp1;
					temp1.setFillColor(sf::Color::Red);
					temp1.setRadius(4);
					temp1.setOrigin(2, 2);
					temp1.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
					(*(this->window)).draw(temp1);
					if (det.contactpointcount == 2)
					{
						sf::CircleShape temp2;
						temp2.setFillColor(sf::Color::Red);
						temp2.setRadius(4);
						temp2.setOrigin(2, 2);
						temp2.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
						(*(this->window)).draw(temp2);
					}
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);

				}
			}
			else if (((*itr1)->shapetype == RigidBody::Polygon) && ((*itr2)->shapetype == RigidBody::Rectangle))
			{
				RigidPolygonShape* first = static_cast<RigidPolygonShape*>(*itr1);
				RigidRectangleShape* second = static_cast<RigidRectangleShape*>(*itr2);
				collisionresult result = this->checkcollisionpolygonpolygon(first->position, first->points, second->position, second->points);
				if (result.iscollided)
				{
					first->iscolliding = true;
					second->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);

					contactdetail det = this->findcontactdetailpolygonpolygon(first->points, second->points);


					//drawing collision point
					sf::CircleShape temp1;
					temp1.setFillColor(sf::Color::Red);
					temp1.setRadius(4);
					temp1.setOrigin(2, 2);
					temp1.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
					(*(this->window)).draw(temp1);
					if (det.contactpointcount == 2)
					{
						sf::CircleShape temp2;
						temp2.setFillColor(sf::Color::Red);
						temp2.setRadius(4);
						temp2.setOrigin(2, 2);
						temp2.setPosition(sf::Vector2f(det.contactpoint1.x, det.contactpoint1.y));
						(*(this->window)).draw(temp2);
					}
					this->resolvecollisionwithfriciton((*itr1), (*itr2), result, det);

				}
			}
		}
	}
}

void PhysicsWorld::resolvecollisionwithfriciton(RigidBody* a, RigidBody* b, collisionresult collision, contactdetail contact)
{

	pum::vector2d normal = collision.axis;
	pum::vector2d contact1 = contact.contactpoint1;
	pum::vector2d contact2 = contact.contactpoint2;
	int contactcount = contact.contactpointcount;

	float e = this->coeffofrestitution;

	std::vector<pum::vector2d>contaclist(2);
	contaclist[0] = contact1;
	contaclist[1] = contact2;
	std::vector<pum::vector2d>impulselist(2);
	std::vector<pum::vector2d>frictionimpulselist(2);
	std::vector<pum::vector2d>ralist(2);
	std::vector<pum::vector2d>rblist(2);
	std::vector<double>jlist(2);

	for (int i = 0; i < contactcount; i++)
	{
		pum::vector2d ra = contaclist[i] - a->position;
		pum::vector2d rb = contaclist[i] - b->position;

		ralist[i] = ra;
		rblist[i] = rb;

		pum::vector2d raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		pum::vector2d rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);
		pum::vector2d angularlinearvelocitya = raperp * (a->deg2rad(a->angularvelocity));
		pum::vector2d angularlinearvelocityb = rbperp * (b->deg2rad(b->angularvelocity)); 
		pum::vector2d relativevelocity = (b->velocity + angularlinearvelocityb) - (a->velocity + angularlinearvelocitya);

		float contactvelocitymag = pum::dotpro(relativevelocity, normal);
		/*std::cout << normal.x << " " << normal.y << std::endl;
		std::cout << contactvelocitymag << std::endl;
		std::cout << std::endl;*/
		if (contactvelocitymag < 0.0)
		{
			continue;
		}

		double raperpdotn = pum::dotpro(raperp, normal);
		double rbperpdotn = pum::dotpro(rbperp, normal);

		double denom = a->getInvMass() + b->getInvMass() +
			(raperpdotn * raperpdotn) * a->getInvInertia() +
			(rbperpdotn * rbperpdotn) * b->getInvInertia();

		double j = -1.0 * (1.0 + e) * contactvelocitymag;
		j = j / denom;
		j = j / (double)contactcount;

		jlist[i] = j;

		pum::vector2d impulse = normal * j;
		impulselist[i] = impulse;

	}

	for (int i = 0; i < contactcount; i++)
	{
		pum::vector2d impulse = impulselist[i];
		pum::vector2d ra = ralist[i];
		pum::vector2d rb = rblist[i];

		pum::vector2d raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		pum::vector2d rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);

		a->velocity = a->velocity - (impulse * a->getInvMass());
		pum::vector2d achange = (impulse * a->getInvMass() * -1);
		//std::cout << "achange " << achange.x << " " << achange.y << std::endl;
		a->angularvelocity -= a->rad2deg(pum::dotpro(raperp, impulse)) * a->getInvInertia();

		b->velocity = b->velocity + (impulse * b->getInvMass());
		pum::vector2d bchange = (impulse * b->getInvMass() * 1);
		//std::cout << "bchange " << bchange.x << " " << bchange.y << std::endl;
		b->angularvelocity += b->rad2deg(pum::dotpro(rbperp, impulse)) * b->getInvInertia();
	}

	//return;
	for (int i = 0; i < contactcount; i++)
	{
		pum::vector2d ra = contaclist[i] - a->position;
		pum::vector2d rb = contaclist[i] - b->position;

		ralist[i] = ra;
		rblist[i] = rb;

		pum::vector2d raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		pum::vector2d rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);
		pum::vector2d angularlinearvelocitya = raperp * (a->deg2rad(a->angularvelocity));
		pum::vector2d angularlinearvelocityb = rbperp * (b->deg2rad(b->angularvelocity));
		pum::vector2d relativevelocity = (b->velocity + angularlinearvelocityb) - (a->velocity + angularlinearvelocitya);

		float contactvelocitymag = pum::dotpro(relativevelocity, normal);
		pum::vector2d tangent = relativevelocity - (normal * contactvelocitymag);
		//std::cout << "tangetwith mag " << tangent.x << " " << tangent.y << std::endl;
		if (this->nearlyequal(tangent, pum::vector2d(0, 0)))
		{
			//std::cout << "tangent equal to zero" << std::endl;
			continue;
		}
		else
		{
			//std::cout << "tangent not equal to zero" << std::endl;
			tangent.normalize();
		}

		double raperpdott = pum::dotpro(raperp, tangent);
		double rbperpdott = pum::dotpro(rbperp, tangent);

		double denom = a->getInvMass() + b->getInvMass() +
			(raperpdott * raperpdott) * a->getInvInertia() +
			(rbperpdott * rbperpdott) * b->getInvInertia();

		double jf = -1.0 * pum::dotpro(relativevelocity, tangent);
		jf = jf / denom;
		jf = jf / (double)contactcount;

		pum::vector2d frictionimpulse;
		double j = jlist[i];

		double sf = std::min(a->staticfriction, b->staticfriction);

		if (std::abs(jf) <= abs(j * sf))
		{
			frictionimpulse = tangent * jf;
		}
		else
		{
			double sign = jf / std::abs(jf);
			frictionimpulse = tangent * (sign * abs(j) * sf);
		}
		frictionimpulselist[i] = frictionimpulse;
	}

	for (int i = 0; i < contactcount; i++)
	{
		pum::vector2d frictionimpulse = frictionimpulselist[i];
		pum::vector2d ra = ralist[i];
		pum::vector2d rb = rblist[i];


		pum::vector2d raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		pum::vector2d rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);

		a->velocity = a->velocity - (frictionimpulse * a->getInvMass());
		pum::vector2d achange = (frictionimpulse * a->getInvMass() * -1);
		//std::cout << "achange " << achange.x << " " << achange.y << std::endl;
		a->angularvelocity -= a->rad2deg(pum::dotpro(raperp, frictionimpulse)) * a->getInvInertia();

		b->velocity = b->velocity + (frictionimpulse * b->getInvMass());
		pum::vector2d bchange = (frictionimpulse * b->getInvMass() * -1);
		//std::cout << "bchange " << bchange.x << " " << bchange.y << std::endl;
		b->angularvelocity += b->rad2deg(pum::dotpro(rbperp, frictionimpulse)) * b->getInvInertia();

	}
	//std::cout << "==========" << std::endl;

}

void PhysicsWorld::calculatemovement(double delta)
{
	for (auto it : PhysicsWorld::getInstance()->bodies)
	{

		if (it->bodyType == RigidBody::Static)
		{
			it->velocity = it->velocity + (it->acceleration * delta);
			if (this->nearlyequal(it->velocity, pum::vector2d(0, 0)))
			{
				it->velocity = pum::vector2d(0, 0);
			}
		}
		else
		{
			it->velocity = it->velocity + ((it->acceleration + this->gravity) * delta);
			if (this->nearlyequal(it->velocity, pum::vector2d(0, 0)))
			{
				it->velocity = pum::vector2d(0, 0);
			}
		}
		if (it->shapetype == RigidBody::shapetypes::Circle)
		{
			it->translate(it->velocity * delta);
			it->rotate(it->angularvelocity * delta);
		}
		else if (it->shapetype == RigidBody::shapetypes::Rectangle)
		{
			it->translate(it->velocity * delta);
			it->rotate(it->angularvelocity * delta);
		}
		else if (it->shapetype == RigidBody::shapetypes::Polygon)
		{
			it->translate(it->velocity * delta);
			it->rotate(it->angularvelocity * delta);
		}
	}
}


PhysicsWorld::collisionresult PhysicsWorld::checkcollisionpolygoncircle(pum::vector2d centerrect, std::vector<pum::vector2d>& points, pum::vector2d center, double radius)
{
	PhysicsWorld::collisionresult result;
	for (int point = 0; point < points.size(); point++)
	{
		pum::vector2d a = points[point];
		pum::vector2d b = points[(point + 1) % points.size()];
		pum::vector2d axis = b - a; //finding the egde
		axis = pum::vector2d(axis.y, axis.x * -1); //making it perpendicular to actual finding an axis
		axis.normalize();
		
		std::pair<double, double>polygon = pum::projectpolygon(axis, points);
		std::pair<double, double>circle = pum::projectcircle(axis, center, radius);

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
		axis.normalize();
		polygon = pum::projectpolygon(axis, points);
		circle = pum::projectcircle(axis, center, radius);

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

	pum::vector2d direction = centerrect - center;
	if (pum::dotpro(direction, result.axis) < 0.0)
	{
		result.axis = result.axis * (-1);
	}
	result.iscollided = true;
	return result;


}

PhysicsWorld::collisionresult PhysicsWorld::checkcollisionpolygonpolygon(pum::vector2d center1, std::vector<pum::vector2d>& points1, pum::vector2d center2, std::vector<pum::vector2d>& points2)
{
	PhysicsWorld::collisionresult result;
	for (int point = 0; point < points1.size(); point++)
	{
		pum::vector2d a = points1[point];
		pum::vector2d b = points1[(point + 1) % points1.size()];
		pum::vector2d axis = b - a;
		axis.normalize();
		axis = pum::vector2d(axis.y, axis.x * -1);

		std::pair<double, double>polygon1 = pum::projectpolygon(axis, points1);
		std::pair<double, double>polygon2 = pum::projectpolygon(axis, points2);

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
		pum::vector2d a = points2[point];
		pum::vector2d b = points2[(point + 1) % points2.size()];
		pum::vector2d axis = b - a;
		axis.normalize();
		axis = pum::vector2d(axis.y, axis.x * -1);

		std::pair<double, double>polygon1 = pum::projectpolygon(axis, points1);
		std::pair<double, double>polygon2 = pum::projectpolygon(axis, points2);

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
			result.axis = axis * 1;
		}
	}

	pum::vector2d direction = center1 - center2;
	if (pum::dotpro(direction, result.axis) < 0.0)
	{
		result.axis = result.axis * (-1);
	}
	result.iscollided = true;
	return result;
}

PhysicsWorld::collisionresult PhysicsWorld::checkcollisioncirclecircle(pum::vector2d center1, double radius1, pum::vector2d center2, double radius2)
{
	PhysicsWorld::collisionresult result;
	pum::vector2d axis = center1 - center2;
	double distance = axis.length();
	axis.normalize();
	double radiisum = radius1 + radius2;
	result.iscollided = false;
	if (distance < (radiisum))
	{
		result.iscollided = true;
		axis.normalize();
		result.depth = (radiisum - distance);
		result.axis = axis;

		pum::vector2d direction = center1 - center2;
		if (pum::dotpro(direction, result.axis) < 0.0)
		{
			result.axis = result.axis * (-1);
		}
		result.iscollided = true;
	}
	return result;
}

PhysicsWorld::contactdetail PhysicsWorld::findcontactdetailpolygonpolygon(std::vector<pum::vector2d> points1, std::vector<pum::vector2d> points2)
{
	contactdetail result;
	result.contactpointcount = 0;
	double mindis = LLONG_MAX;


	for (int point1 = 0; point1 < points1.size(); point1++)
	{
		pum::vector2d p = points1[point1];
		for (int point2 = 0; point2 < points2.size(); point2++)
		{
			pum::vector2d a = points2[point2];
			pum::vector2d b = points2[(point2 + 1) % points2.size()];

			pum::pointlineprojectresult tempres = pum::distanceFromLinesegment(a, b, p);
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
		pum::vector2d p = points2[point2];
		for (int point1 = 0; point1 < points1.size(); point1++)
		{
			pum::vector2d a = points1[point1];
			pum::vector2d b = points1[(point1 + 1) % points1.size()];

			pum::pointlineprojectresult tempres = pum::distanceFromLinesegment(a, b, p);
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

PhysicsWorld::contactdetail PhysicsWorld::findcontactdetailpolygoncircle(std::vector<pum::vector2d> points, pum::vector2d center, double radius)
{
	contactdetail result;
	result.contactpointcount = 0;
	double mindis = LLONG_MAX;

	pum::vector2d p = center;
	for (int point = 0; point < points.size(); point++)
	{
		pum::vector2d a = points[point];
		pum::vector2d b = points[(point + 1) % points.size()];

		pum::pointlineprojectresult tempres = pum::distanceFromLinesegment(a, b, p);
		if (mindis > tempres.distance)
		{
			result.contactpoint1 = tempres.projectionpoint;
			result.contactpointcount = 1;
			mindis = tempres.distance;
		}
	}
	return result;
}

PhysicsWorld::contactdetail PhysicsWorld::findcontactdetailcirclecircle(pum::vector2d center1, double radius1, pum::vector2d center2, double radius2)
{
	contactdetail result;
	result.contactpointcount = 0;

	pum::vector2d axis = center1 - center2;
	axis.normalize();
	pum::vector2d p = center2 + (axis * radius2);

	result.contactpoint1 = p;
	result.contactpointcount = 1;
	return result;

}


bool PhysicsWorld::nearlyequal(pum::vector2d a, pum::vector2d b)
{
	pum::vector2d axis = a - b;
	double mag = axis.length();
	if (mag <= 0.005)
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

void PhysicsWorld::separateBodies(RigidBody* a, RigidBody* b, pum::vector2d axis, double depth)
{
	if (a->bodyType == RigidBody::Static)
	{
		b->translate(axis *  (depth * (-1.0)));
	}
	else if (b->bodyType == RigidBody::Static)
	{
		a->translate(axis *  (depth * 1.0));
	}
	else
	{
		a->translate(axis *  (depth * 0.5));
		b->translate(axis *  (depth * -0.5));
	}
}
