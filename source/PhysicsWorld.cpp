#include "../header/PhysicsWorld.h"

#include <iostream>
#include "../header/RigidPolygonShape.h"

using namespace std;
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
	int maxiterations = 80;
	delta = delta / (double)maxiterations;
	for (int i = 0; i < maxiterations; i++)
	{
		calculatemovement(delta);
		collisionDetection();
		constraintsolving(delta);
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

			if (((((*itr1)->mask) & ((*itr2)->layer)).count() == 0) && ((((*itr1)->layer) & ((*itr2)->mask)).count() == 0))
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
					//cout << "axis " << result.axis.x << " " << result.axis.y << endl;
					first->iscolliding = true;
					second->iscolliding = true;
					this->separateBodies((*itr1), (*itr2), result.axis, result.depth);

					contactdetail det = this->findcontactdetailpolygonpolygon(first->points, second->points);

					//cout << "contact count " << det.contactpointcount << endl;
					//drawing collision point
					sf::CircleShape temp1;
					temp1.setFillColor(sf::Color::Red);
					temp1.setRadius(5);
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

void PhysicsWorld::constraintsolving(double delta)
{
	for (auto& it : joints)
	{
		solvejointswithfriction2(it);
	}

	for (auto& it : springs)
	{
		solvespring(it,delta);
	}
}


void PhysicsWorld::solvejointswithfriction(joint* nail)
{
	// if there are zero or only one bodies are on the nail then skipping the next parts
	if (nail->bodies.size() <= 1)
	{
		return;
	}

	//correcting the positions of the objects
	long double masssum = 0;
	pum::vector2d jointpos;
	bool issomebodystatic = false;
	for (auto& it : nail->bodies)
	{
		if (it.second->bodyType == RigidBody::Static)
		{
			issomebodystatic = true;
			jointpos = (it.second)->getglobalpos(it.first);
			break;
		}
		pum::vector2d currpos = (it.second)->getglobalpos(it.first);
		jointpos = jointpos + (currpos * ((it.second)->mass));
		masssum += ((it.second)->mass);
	}
	if (issomebodystatic == false)
	{
		jointpos = jointpos / masssum;
	}
	//cout << "joint pos " << jointpos.x<<" "<<jointpos.y << endl;
	//correcting the position
	for (auto itr = begin(nail->bodies); itr != end(nail->bodies); itr++)
	{
		//cout << "global pos " << ((itr->second)->getglobalpos(itr->first)).x << " " << ((itr->second)->getglobalpos(itr->first)).y << endl;
		pum::vector2d posdiff = jointpos - (itr->second)->getglobalpos(itr->first);
		//to remove float point error
		//cout << "joint xfact " << jointpos.x << " globalxfact " << ((itr->second)->getglobalpos(itr->first)).x << endl;
		posdiff.x = jointpos.x - ((itr->second)->getglobalpos(itr->first)).x;
		//cout << "joint yfact " << jointpos.y << " globalyfact " << ((itr->second)->getglobalpos(itr->first)).y << endl;
		posdiff.y = jointpos.y - ((itr->second)->getglobalpos(itr->first)).y;

		/*if (this->nearlyequal(jointpos, pum::vector2d(0, 0)))
		{
			jointpos = pum::vector2d(0, 0);
		}*/
		//posdiff = this->simplify(posdiff);
		//pos
		//cout << "posdiff " << posdiff.x << " " << posdiff.y << endl;
		//(itr->second)->position = (itr->second)->position + posdiff;
		(itr->second)->setpositon((itr->second)->position + posdiff);
	}
	//drawing the joint point for debug
	sf::CircleShape temp;
	temp.setFillColor(sf::Color::Yellow);
	temp.setRadius(4);
	temp.setOrigin(2, 2);
	temp.setPosition(sf::Vector2f(jointpos.x, jointpos.y));
	(*(this->window)).draw(temp);
	//std::cout << "solving joint with friction " << nail->staticfricitonconst << std::endl;
	std::pair<pum::vector2d, RigidBody*> basebodydet = nail->bodies[0];
	for (int i = 1; i < nail->bodies.size(); i++)
	{
		std::pair<pum::vector2d, RigidBody*> currbodydet = (nail->bodies)[i];
		pum::vector2d normal = basebodydet.second->getglobalpos(basebodydet.first) - currbodydet.second->getglobalpos(currbodydet.first);
		normal.normalize();

		pum::vector2d ra = jointpos - (basebodydet.second->position);
		pum::vector2d rb = jointpos - (currbodydet.second->position);
		//std::cout << "rb " << rb.x << " " << rb.y << endl;
		pum::vector2d raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		pum::vector2d rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);

		//(currbodydet.second)->velocity = pum::vector2d(73.8838 , -15.6203);
		//(currbodydet.second)->velocity = pum::vector2d(-15.6203 , -15.6203);
		//(currbodydet.second)->angularvelocity = -89.62;
		//(currbodydet.second)->angularvelocity = 0;
		pum::vector2d angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		pum::vector2d angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		pum::vector2d relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) - ((basebodydet.second)->velocity + angularlinearvelocitya);

		
		normal = relativevelocity;
		normal.normalize();
		//normal = normal * -1;
		//relativevelocity.makelen(18.8534);
		//std::cout << "relvel orig " << (relativevelocity.x) << " " << relativevelocity.y << std::endl;
		//std::cout <<"relvel orig length "<<(relativevelocity.length()) << std::endl;
		//std::cout <<"normal "<< normal.x << " " << normal.y << std::endl;

		long double contactvelocitymag = pum::dotpro(relativevelocity, normal);
		//std::cout << "contact vel " << contactvelocitymag << std::endl;
		/*if (normal == pum::vector2d(0, 0))
		{
			continue;
		}
		if (contactvelocitymag == 0.0)
		{
			continue;
		}*/
		long double raperpdotn = pum::dotpro(raperp, normal);
		long double rbperpdotn = pum::dotpro(rbperp, normal);

		long double denom = (basebodydet.second)->getInvMass() + (currbodydet.second)->getInvMass() +
			(raperpdotn * raperpdotn) * (basebodydet.second)->getInvInertia() +
			(rbperpdotn * rbperpdotn) * (currbodydet.second)->getInvInertia();
		//long double denom = (currbodydet.second)->getInvMass() + (rbperpdotn * rbperpdotn) * (currbodydet.second)->getInvInertia();
		//std::cout << "rbperdotn " << rbperpdotn << std::endl;
		//std::cout << "invmass " << ((currbodydet.second)->getInvMass()) << "  inv inertia " << ((currbodydet.second)->getInvInertia()) << std::endl;
		//std::cout << "denom " << denom << std::endl;
		long double e = this->coeffofrestitution;
		//e = 0;
		long double j = -1.0 * (1.0 + e) * contactvelocitymag;
		j = j / denom;
		//std::cout << "j " << j << std::endl;
		pum::vector2d impulse = normal * j;
		//std::cout << "impulse " <p< impulse.x<<" "<<impulse.y << std::endl;
		//std::cout << "prev vel basebody  " << ((basebodydet.second)->velocity).x<<" "<< ((basebodydet.second)->velocity).y << std::endl;
		(basebodydet.second)->velocity = (basebodydet.second)->velocity - (impulse * (basebodydet.second)->getInvMass());
		//std::cout << "next vel basebody " << ((basebodydet.second)->velocity).x<<" "<< ((basebodydet.second)->velocity).y << std::endl;
		pum::vector2d achange = (impulse * (basebodydet.second)->getInvMass() * -1);
		//std::cout << "prev angular vel basebody " << ((basebodydet.second)->angularvelocity) << std::endl;
		//std::cout << "raperp " << raperp.x << " " << raperp.y << std::endl;
		(basebodydet.second)->angularvelocity -= (basebodydet.second)->rad2deg(pum::dotpro(raperp, impulse)) * (basebodydet.second)->getInvInertia();
		//std::cout << "next angular vel basebody " << (((basebodydet.second)->angularvelocity)) << std::endl;

		//std::cout << "prev vel " << ((currbodydet.second)->velocity).x<<" "<< ((currbodydet.second)->velocity).y << std::endl;
		(currbodydet.second)->velocity = (currbodydet.second)->velocity + (impulse * (currbodydet.second)->getInvMass());
		//std::cout << "prev vel " << ((currbodydet.second)->velocity) << std::endl;
		//std::cout << "next vel " << ((currbodydet.second)->velocity).x<<" "<< ((currbodydet.second)->velocity).y << std::endl;
		pum::vector2d bchange = (impulse * (currbodydet.second)->getInvMass() * -1);
		//std::cout << "prev angular vel " << ((currbodydet.second)->angularvelocity) << std::endl;
		//std::cout << "rbperp " << rbperp.x << " " << rbperp.y << std::endl;
		(currbodydet.second)->angularvelocity += (currbodydet.second)->rad2deg(pum::dotpro(rbperp, impulse)) * (currbodydet.second)->getInvInertia();
		//std::cout << "next angular vel " << (((currbodydet.second)->angularvelocity)) << std::endl;


		//std::cout << "rbperp " << rbperp.x << " " << rbperp.y << std::endl;
		angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		//cout << "angulatlinear velocity " << (angularlinearvelocityb.x) << " " << (angularlinearvelocityb.y) << endl;
		relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) -((basebodydet.second)->velocity + angularlinearvelocitya);
		normal = relativevelocity;
		normal.normalize();
		//std::cout << "relvel " << (relativevelocity.x) << " " << relativevelocity.y << std::endl;
		//std::cout<<"rel vel length "<<(relativevelocity.length())<<std::endl;
		//std::cout << "contact vel length " << (pum::dotpro(relativevelocity, normal)) << std::endl;
		//std::cout << std::endl;


		//return;
		continue;

		//starting the friction calculation from here......
		pum::vector2d contactpoint1 = jointpos - (normal * (nail->radius));


		ra = contactpoint1 - (basebodydet.second)->position;
		rb = contactpoint1 - (currbodydet.second)->position;

		raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);

		angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) - ((basebodydet.second)->velocity + angularlinearvelocitya);

		contactvelocitymag = pum::dotpro(relativevelocity, normal);


		pum::vector2d tangent = relativevelocity - (normal * contactvelocitymag);
		//std::cout << "static relvel orig " << (relativevelocity.length()) << std::endl;
		//std::cout << "tangetwith mag " << tangent.x << " " << tangent.y << std::endl;
		if (this->nearlyequal(tangent, pum::vector2d(0, 0)))
		{
			continue;
		}
		else
		{
			tangent.normalize();
		}


		double raperpdott = pum::dotpro(raperp, tangent);
		double rbperpdott = pum::dotpro(rbperp, tangent);

		denom = (basebodydet.second)->getInvMass() + (currbodydet.second)->getInvMass() +
			(raperpdott * raperpdott) * (basebodydet.second)->getInvInertia() +
			(rbperpdott * rbperpdott) * (currbodydet.second)->getInvInertia();

		//std::cout << "contact vel friction " << pum::dotpro(relativevelocity, tangent) << std::endl;
		double jf = -1.0 * pum::dotpro(relativevelocity, tangent);
		jf = jf / denom;

		pum::vector2d frictionimpulse;

		double sf = nail->staticfricitonconst;// std::min((basebodydet.second)->staticfriction, (currbodydet.second)->staticfriction);
		//cout << "impulse needed " << abs(jf) << endl;
		//cout << "impulse provided " << (abs(j * sf))<<" j= "<<j << endl;
		if (std::abs(jf) <= abs(j * sf))
		{
			//std::cout << "enough friciton" << std::endl;
			frictionimpulse = tangent * jf;
		}
		else
		{
			//std::cout << "not enough friciton" << std::endl;
			double sign = jf / std::abs(jf);
			frictionimpulse = tangent * (sign * abs(j) * sf);
		}


		(basebodydet.second)->velocity = (basebodydet.second)->velocity - (frictionimpulse * (basebodydet.second)->getInvMass());
		achange = (frictionimpulse * (basebodydet.second)->getInvMass() * -1);
		//std::cout << "avel " << ((basebodydet.second)->velocity.x) << " " << ((basebodydet.second)->velocity.y) << std::endl;
		//std::cout << "achange " << achange.x << " " << achange.y << std::endl;
		(basebodydet.second)->angularvelocity -= (basebodydet.second)->rad2deg(pum::dotpro(raperp, frictionimpulse)) * (basebodydet.second)->getInvInertia();


		//std::cout << "bvel prev " << ((currbodydet.second)->velocity.x) << " " << ((currbodydet.second)->velocity.y) << std::endl;
		(currbodydet.second)->velocity = (currbodydet.second)->velocity + (frictionimpulse * (currbodydet.second)->getInvMass());
		bchange = (frictionimpulse * (currbodydet.second)->getInvMass() * -1);
		//std::cout << "bvel " << ((currbodydet.second)->velocity.x) << " " << ((currbodydet.second)->velocity.y) << std::endl;
		//std::cout << "bchange " << bchange.x << " " << bchange.y << std::endl;
		(currbodydet.second)->angularvelocity += (currbodydet.second)->rad2deg(pum::dotpro(rbperp, frictionimpulse)) * (currbodydet.second)->getInvInertia();
		//std::cout << "a angulat vel " << ((currbodydet.second)->angularvelocity) << std::endl;
		
		angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) - ((basebodydet.second)->velocity + angularlinearvelocitya);
		

		//std::cout << "static friciton " << (nail->staticfricitonconst) << " " << (relativevelocity.length()) << std::endl;
		//cout << "contact vel rem " << pum::dotpro(relativevelocity, tangent) << endl;


	}
	//cout << endl;
	//std::cout << " ============= " << std::endl;




}

void PhysicsWorld::solvejointswithfriction2(joint* nail)
{
	// if there are zero or only one bodies are on the nail then skipping the next parts
	if (nail->bodies.size() <= 1)
	{
		return;
	}

	//correcting the positions of the objects
	long double masssum = 0;
	pum::vector2d jointpos;
	bool issomebodystatic = false;
	for (auto& it : nail->bodies)
	{
		if (it.second->bodyType == RigidBody::Static)
		{
			issomebodystatic = true;
			jointpos = (it.second)->getglobalpos(it.first);
			break;
		}
		pum::vector2d currpos = (it.second)->getglobalpos(it.first);
		jointpos = jointpos + (currpos * ((it.second)->mass));
		masssum += ((it.second)->mass);
	}
	if (issomebodystatic == false)
	{
		jointpos = jointpos / masssum;
	}
	//correcting the position
	
	//drawing the joint point for debug
	sf::CircleShape temp;
	temp.setFillColor(sf::Color::Yellow);
	temp.setRadius(5);
	temp.setOrigin(2.5, 2.5);
	temp.setPosition(sf::Vector2f(jointpos.x, jointpos.y));
	(*(this->window)).draw(temp);
	std::pair<pum::vector2d, RigidBody*> basebodydet = nail->bodies[0];
	for (int i = 1; i < nail->bodies.size(); i++)
	{
		std::pair<pum::vector2d, RigidBody*> currbodydet = (nail->bodies)[i];
		pum::vector2d normal = basebodydet.second->getglobalpos(basebodydet.first) - currbodydet.second->getglobalpos(currbodydet.first);
		normal.normalize();

		pum::vector2d ra = basebodydet.second->getglobalpos(basebodydet.first) - (basebodydet.second->position);
		pum::vector2d rb = currbodydet.second->getglobalpos(currbodydet.first) - (currbodydet.second->position);

		pum::vector2d raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		pum::vector2d rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);

		pum::vector2d angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		pum::vector2d angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		pum::vector2d relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) - ((basebodydet.second)->velocity + angularlinearvelocitya);

		long double contactvelocitymag = pum::dotpro(relativevelocity, normal);
		
		long double raperpdotn = pum::dotpro(raperp, normal);
		long double rbperpdotn = pum::dotpro(rbperp, normal);

		long double denom = (basebodydet.second)->getInvMass() + (currbodydet.second)->getInvMass() +
			(raperpdotn * raperpdotn) * (basebodydet.second)->getInvInertia() +
			(rbperpdotn * rbperpdotn) * (currbodydet.second)->getInvInertia();
		
		long double e = this->coeffofrestitution;
		
		long double j = -1.0 * (1.0 + 1.0) * contactvelocitymag;
		j = j / denom;
		
		pum::vector2d impulse = normal * j;
		(basebodydet.second)->velocity = (basebodydet.second)->velocity - (impulse * (basebodydet.second)->getInvMass());
		pum::vector2d achange = (impulse * (basebodydet.second)->getInvMass() * -1);
		(basebodydet.second)->angularvelocity -= (basebodydet.second)->rad2deg(pum::dotpro(raperp, impulse)) * (basebodydet.second)->getInvInertia();
		(currbodydet.second)->velocity = (currbodydet.second)->velocity + (impulse * (currbodydet.second)->getInvMass());
		pum::vector2d bchange = (impulse * (currbodydet.second)->getInvMass() * -1);
		(currbodydet.second)->angularvelocity += (currbodydet.second)->rad2deg(pum::dotpro(rbperp, impulse)) * (currbodydet.second)->getInvInertia();
		angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) - ((basebodydet.second)->velocity + angularlinearvelocitya);
		


		//recalculating the normal for friction constant
		normal = relativevelocity;
		normal.normalize();

		//starting the friction calculation from here......
		pum::vector2d contactpoint1 = jointpos - (normal * (nail->radius));


		ra = contactpoint1 - (basebodydet.second)->position;
		rb = contactpoint1 - (currbodydet.second)->position;

		raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
		rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);

		angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) - ((basebodydet.second)->velocity + angularlinearvelocitya);

		contactvelocitymag = pum::dotpro(relativevelocity, normal);


		pum::vector2d tangent = relativevelocity - (normal * contactvelocitymag);
		if (this->nearlyequal(tangent, pum::vector2d(0, 0)))
		{
			continue;
		}
		else
		{
			tangent.normalize();
		}


		double raperpdott = pum::dotpro(raperp, tangent);
		double rbperpdott = pum::dotpro(rbperp, tangent);

		denom = (basebodydet.second)->getInvMass() + (currbodydet.second)->getInvMass() +
			(raperpdott * raperpdott) * (basebodydet.second)->getInvInertia() +
			(rbperpdott * rbperpdott) * (currbodydet.second)->getInvInertia();

		double jf = -1.0 * pum::dotpro(relativevelocity, tangent);
		jf = jf / denom;

		pum::vector2d frictionimpulse;

		double sf = nail->staticfricitonconst;
		if (std::abs(jf) <= abs(j * sf))
		{
			frictionimpulse = tangent * jf;
		}
		else
		{
			double sign = jf / std::abs(jf);
			frictionimpulse = tangent * (sign * abs(j) * sf);
		}


		(basebodydet.second)->velocity = (basebodydet.second)->velocity - (frictionimpulse * (basebodydet.second)->getInvMass());
		achange = (frictionimpulse * (basebodydet.second)->getInvMass() * -1);
		(basebodydet.second)->angularvelocity -= (basebodydet.second)->rad2deg(pum::dotpro(raperp, frictionimpulse)) * (basebodydet.second)->getInvInertia();


		(currbodydet.second)->velocity = (currbodydet.second)->velocity + (frictionimpulse * (currbodydet.second)->getInvMass());
		bchange = (frictionimpulse * (currbodydet.second)->getInvMass() * -1);
		(currbodydet.second)->angularvelocity += (currbodydet.second)->rad2deg(pum::dotpro(rbperp, frictionimpulse)) * (currbodydet.second)->getInvInertia();

		angularlinearvelocitya = raperp * ((basebodydet.second)->deg2rad((basebodydet.second)->angularvelocity));
		angularlinearvelocityb = rbperp * ((currbodydet.second)->deg2rad((currbodydet.second)->angularvelocity));
		relativevelocity = ((currbodydet.second)->velocity + angularlinearvelocityb) - ((basebodydet.second)->velocity + angularlinearvelocitya);

	}

	for (auto itr = begin(nail->bodies); itr != end(nail->bodies); itr++)
	{
		pum::vector2d posdiff = jointpos - (itr->second)->getglobalpos(itr->first);
		(itr->second)->setpositon((itr->second)->position + posdiff);
	}



}

void PhysicsWorld::solvespring(spring* spr,double delta)
{
	pum::vector2d pos1 = spr->body1->getglobalpos(spr->body1point);
	pum::vector2d pos2 = spr->body2->getglobalpos(spr->body2point);

	
	
	//solving distance constraint
	double dis = (pos1 - pos2).length();
	pum::vector2d reldis = pos2 - pos1;
	double tomove = 0;
	
	reldis.normalize();

	separateBodies(spr->body1, spr->body2, reldis, -1.0 *tomove);
	dis = (pos1 - pos2).length();
	reldis = pos2 - pos1;
	double x = dis - spr->length;


	sf::Color col;
	if (x > 0)
	{
		col = sf::Color::Red;
	}
	else
	{
		col = sf::Color::Yellow;
	}
	sf::RectangleShape line(sf::Vector2f(dis, 10));
	line.setFillColor(col);
	line.setOrigin(sf::Vector2f(dis / 2, 5));
	line.setPosition(sf::Vector2f((pos1.x + pos2.x)/2.0,(pos1.y + pos2.y)/2.0));


	double costheta = pum::dotpro(pum::vector2d(dis, 10), reldis) / ((std::sqrt((dis*dis) + (100)) * reldis.length()));

	line.setRotation(pum::rad2deg( std::acos(costheta)));
	this->window->draw(line);

	reldis.normalize();
	spr->springconstant = 9000;

	pum::vector2d impulse = reldis * (spr->springconstant * x * -1 * delta);

	
	pum::vector2d ra = pos1 - spr->body1->position;
	pum::vector2d rb = pos2 - spr->body2->position;

	pum::vector2d raperp = pum::vector2d(-1.0 * ra.y, 1.0 * ra.x);
	pum::vector2d rbperp = pum::vector2d(-1.0 * rb.y, 1.0 * rb.x);
	pum::vector2d angularlinearvelocitya = raperp * (spr->body1->deg2rad(spr->body1->angularvelocity));
	pum::vector2d angularlinearvelocityb = rbperp * (spr->body2->deg2rad(spr->body2->angularvelocity));
	pum::vector2d relativevelocity = (spr->body2->velocity + angularlinearvelocityb) - (spr->body1->velocity + angularlinearvelocitya);
	
	impulse = impulse + (relativevelocity * (spr->coefficientofdamping) * -1.0 * delta);


	spr->body1->applyImpulse(impulse * -1.0, pos1);
	//a->velocity = a->velocity - (impulse * a->getInvMass());
	//a->angularvelocity -= a->rad2deg(pum::dotpro(raperp, impulse)) * a->getInvInertia();

	spr->body2->applyImpulse(impulse * 1.0, pos2);
	//b->velocity = b->velocity + (impulse * b->getInvMass());
	//b->angularvelocity += b->rad2deg(pum::dotpro(rbperp, impulse)) * b->getInvInertia();

	/*
	spr->body1->velocity = spr->body1->velocity - (impulse * spr->body1->getInvMass());
	pum::vector2d achange = (impulse * spr->body1->getInvMass() * -1);
	//std::cout << "achange " << achange.x << " " << achange.y << std::endl;
	spr->body1->angularvelocity -= spr->body1->rad2deg(pum::dotpro(raperp,impulse)) * spr->body1->getInvInertia();


	spr->body2->velocity = spr->body2->velocity + (impulse * spr->body2->getInvMass());
	pum::vector2d bchange = (impulse * spr->body2->getInvMass() * -1);
	//std::cout << "bchange " << bchange.x << " " << bchange.y << std::endl;
	spr->body2->angularvelocity += spr->body2->rad2deg(pum::dotpro(rbperp,impulse)) * spr->body2->getInvInertia();*/



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

		a->applyImpulse(impulse * -1.0, contaclist[i]);
		//a->velocity = a->velocity - (impulse * a->getInvMass());
		//a->angularvelocity -= a->rad2deg(pum::dotpro(raperp, impulse)) * a->getInvInertia();

		b->applyImpulse(impulse * 1.0, contaclist[i]);
		//b->velocity = b->velocity + (impulse * b->getInvMass());
		//b->angularvelocity += b->rad2deg(pum::dotpro(rbperp, impulse)) * b->getInvInertia();
	}


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
			continue;
		}
		else
		{
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



		a->applyImpulse(frictionimpulse * -1.0, contaclist[i]);
		//a->velocity = a->velocity - (impulse * a->getInvMass());
		//a->angularvelocity -= a->rad2deg(pum::dotpro(raperp, impulse)) * a->getInvInertia();

		b->applyImpulse(frictionimpulse * 1.0, contaclist[i]);
		//b->velocity = b->velocity + (impulse * b->getInvMass());
		//b->angularvelocity += b->rad2deg(pum::dotpro(rbperp, impulse)) * b->getInvInertia();


		//a->velocity = a->velocity - (frictionimpulse * a->getInvMass());
		//pum::vector2d achange = (frictionimpulse * a->getInvMass() * -1);
		//a->angularvelocity -= a->rad2deg(pum::dotpro(raperp, frictionimpulse)) * a->getInvInertia();

		//b->velocity = b->velocity + (frictionimpulse * b->getInvMass());
		//pum::vector2d bchange = (frictionimpulse * b->getInvMass() * -1);
		//b->angularvelocity += b->rad2deg(pum::dotpro(rbperp, frictionimpulse)) * b->getInvInertia();

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

pum::vector2d PhysicsWorld::simplify(pum::vector2d& a)
{
	if (abs(a.x) < 0.005)
		a.x = 0;
	if (abs(a.y) < 0.005)
		a.y = 0;
	return a;
}


bool PhysicsWorld::nearlyequal(pum::vector2d a, pum::vector2d b)
{
	pum::vector2d axis = a - b;
	double mag = axis.length();
	//cout << "mag " << mag << endl;
	/*if ((abs(a.x - b.x) < 0.005) && (abs(a.y - b.y) < 0.005))
		return true;*/
	if (mag <= 0.005)
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
