#include "../header/PhysicsEngine.h"
#include <iostream>

int f = 0;
int PhysicsEngine::addObject(RigidCircle* obj)
{
	if (this->unusedindex.size() > 0)
	{
		int lastindex = (this->unusedindex).back();
		(this->indexes)[lastindex] = obj;
		(this->unusedindex).pop_back();
		if (obj->type == RigidCircle::_rigid)
		{
			(this->rigidobjects).insert(obj);
		}
		else
		{
			(this->staticobjects.insert(obj));
		}
		return lastindex;
	}
	else
	{
		(this->indexes).push_back(obj);
		if (obj->type == RigidCircle::_rigid)
		{
			(this->rigidobjects).insert(obj);
		}
		else
		{
			(this->staticobjects.insert(obj));
		}
		this->maxindex += 1;
		return (this->indexes).size() - 1;
	}
	return -1;
}

void PhysicsEngine::removeObject(int index)
{
	//complete this function
}

void PhysicsEngine::process(double delta)
{
	f++;
	int maxiterations = 8;
	delta = delta / (double)maxiterations;
	for (int i = 0; i < maxiterations; i++)
	{

		//this->solveCollisionCumulative();
		this->solveCollisionDirect();
		calculatemovement(delta);
		
	}
}

void PhysicsEngine::calculatemovement(double delta)
{
	for (auto& it : rigidobjects)
	{
		it->velocity = it->velocity + this->mult(this->gravity + it->acceleration, delta);
		it->pos = it->pos + this->mult(it->velocity, delta);
	}

	for (auto& it : staticobjects)
	{
		it->velocity = it->velocity + this->mult(it->acceleration, delta);
		it->pos = it->pos + this->mult(it->velocity, delta);
	}
}

void PhysicsEngine::solveCollisionCumulative()
{

	double resolutionfactor = 0.8;
	int maxiter = 30;

	for (int i = 0; i < maxiter; i++)
	{
		for (auto& it : this->rigidobjects)
		{
			//checking for collision with the bounding box
			if ((it->pos.x + it->radius) > bh->right)
			{
				it->velocity.x = -(this->coeffofrestitution) * abs(it->velocity.x);
				double diff = it->pos.x + it->radius - bh->right;
				it->pos.x = it->pos.x - (diff * resolutionfactor);
			}
			if ((it->pos.x - it->radius) < (bh->left))
			{
				it->velocity.x = this->coeffofrestitution* abs(it->velocity.x);
				double diff = bh->left - (it->pos.x - it->radius);
				it->pos.x = it->pos.x + (diff * resolutionfactor);
			}
			if ((it->pos.y + it->radius) > (bh->bottom))
			{
				it->velocity.y = (-(this->coeffofrestitution) * abs(it->velocity.y));
				double diff = (it->pos.y + it->radius) - (bh->bottom);
				it->pos.y = it->pos.y - (diff * resolutionfactor);
			}
			if ((it->pos.y - it->radius) < (bh->top))
			{
				it->velocity.y =(this->coeffofrestitution)* abs(it->velocity.y);
				double diff = bh->top - (it->pos.y - it->radius);
				it->pos.y = it->pos.y + (diff * resolutionfactor);
			}
		}

		//detecting collision between rigid bodies and static bodies
		std::vector<std::pair<RigidCircle*, RigidCircle*>>staticcollision;
		for (auto itr = this->rigidobjects.begin(); itr != this->rigidobjects.end(); itr++)
		{
			for (auto itr2 = this->staticobjects.begin(); itr2 != this->staticobjects.end(); itr2++)
			{
				sf::Vector2f impact = (*itr)->pos - (*itr2)->pos;
				double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));
				if (length < ((*itr)->radius + (*itr2)->radius))
				{
					//std::cout << "collided " << std::endl;
					staticcollision.push_back({ (*itr),(*itr2) });
				}
			}
		}

		//solving the collision between rigid bodies and static bodies
		for (auto& it : staticcollision)
		{
			RigidCircle* rigid = it.first;
			RigidCircle* stat = it.second;
			sf::Vector2f impact = rigid->pos - stat->pos;
			double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));

			double overlap = (rigid->radius + stat->radius) - length;
			sf::Vector2f dir = impact;
			dir = this->setmag(dir, overlap * resolutionfactor);
			rigid->pos = (rigid->pos) + dir;
			impact = rigid->pos - stat->pos;

			sf::Vector2f othervalrem = this->mult(impact, (this->dotpro(stat->velocity, impact) / (length * length)));
			sf::Vector2f thisvalrem = this->mult(impact, (this->dotpro(rigid->velocity, impact) / (length * length)));
			rigid->velocity = rigid->velocity - thisvalrem;


			sf::Vector2f deltathis = this->mult((othervalrem - thisvalrem),  this->coeffofrestitution) + thisvalrem;
			rigid->velocity = rigid->velocity + deltathis;
			


		}


		//for other bodies
		std::vector<std::pair<RigidCircle*, RigidCircle*>>collided;
		for (auto itr = this->rigidobjects.begin(); itr != this->rigidobjects.end(); itr++)
		{
			for (auto itr2 = next(itr); itr2 != this->rigidobjects.end(); itr2++)
			{
				sf::Vector2f impact = (*itr)->pos - (*itr2)->pos;
				double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));
				if (length < ((*itr)->radius + (*itr2)->radius))
				{
					collided.push_back({ (*itr),(*itr2) });
				}
			}
		}
		for (auto& it : collided)
		{
			RigidCircle* first = it.first;
			RigidCircle* second = it.second;

			sf::Vector2f impact = first->pos - second->pos;
			double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));

			double overlap = (first->radius + second->radius) - length;
			sf::Vector2f dir = impact;
			dir = this->setmag(dir, (overlap / 2.0) * (resolutionfactor));

			//distance correction
			first->pos = (first->pos) + dir;
			second->pos = (second->pos) - dir;
			
			length = first->radius + second->radius;
			impact = this->setmag(impact, length);


			//removing the velocity vector along the impact line for both the balls
			sf::Vector2f othervalrem = this->mult(impact ,(this->dotpro(second->velocity, impact) / (length * length)));
			second->velocity = second->velocity - othervalrem;
			sf::Vector2f thisvalrem = this->mult(impact, (this->dotpro(first->velocity,impact) / (length * length)));
			first->velocity = first->velocity - thisvalrem;

		
			sf::Vector2f totalmomentum = this->mult(thisvalrem,first->mass) + this->mult(othervalrem,second->mass);
			double msum = first->mass + second->mass;

			sf::Vector2f deltathis = totalmomentum + this->mult((othervalrem - thisvalrem),(second->mass * this->coeffofrestitution));
			deltathis = this->mult(deltathis, (1.0/msum));
			first->velocity = first->velocity + deltathis;

			sf::Vector2f deltaother = totalmomentum + this->mult((thisvalrem - othervalrem), (first->mass * this->coeffofrestitution));
			deltaother = this->mult(deltaother, (1.0 / msum));
			second->velocity = second->velocity + deltaother;

		}

	}
}



void PhysicsEngine::solveCollisionDirect()
{

	for (auto& it : this->rigidobjects)
	{
		//checking for collision with the bounding box
		if ((it->pos.x + it->radius) > bh->right)
		{
			it->velocity.x = -(this->coeffofrestitution) * abs(it->velocity.x);
			double diff = it->pos.x + it->radius - bh->right;
			it->pos.x = it->pos.x - (diff);
		}
		if ((it->pos.x - it->radius) < (bh->left))
		{
			//std::cout << "collidec left" << std::endl;
			it->velocity.x = this->coeffofrestitution * abs(it->velocity.x);
			double diff = bh->left - (it->pos.x - it->radius);
			it->pos.x = it->pos.x + (diff);
		}
		if ((it->pos.y + it->radius) > (bh->bottom))
		{
			//std::cout << "collidec bottom" << std::endl;
			it->velocity.y = (-(this->coeffofrestitution) * abs(it->velocity.y));
			double diff = (it->pos.y + it->radius) - (bh->bottom);
			it->pos.y = it->pos.y - (diff);
		}
		if ((it->pos.y - it->radius) < (bh->top))
		{
			//std::cout << "collidec top" << std::endl;
			it->velocity.y = (this->coeffofrestitution) * abs(it->velocity.y);
			double diff = bh->top - (it->pos.y - it->radius);
			it->pos.y = it->pos.y + (diff);
		}
	}


	//detecting collision between rigid bodies and static bodies
	std::vector<std::pair<RigidCircle*, RigidCircle*>>staticcollision;
	for (auto itr = this->rigidobjects.begin(); itr != this->rigidobjects.end(); itr++)
	{
		for (auto itr2 = this->staticobjects.begin(); itr2 != this->staticobjects.end(); itr2++)
		{
			sf::Vector2f impact = (*itr)->pos - (*itr2)->pos;
			double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));
			if (length < ((*itr)->radius + (*itr2)->radius))
			{
				//std::cout << "collided " << std::endl;
				staticcollision.push_back({ (*itr),(*itr2) });
			}
		}
	}

	//solving the collision between rigid bodies and static bodies
	for (auto& it : staticcollision)
	{
		RigidCircle* rigid = it.first;
		RigidCircle* stat = it.second;
		sf::Vector2f impact = rigid->pos - stat->pos;
		double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));

		double overlap = (rigid->radius + stat->radius) - length;
		sf::Vector2f dir = impact;
		dir = this->setmag(dir, overlap);
		rigid->pos = (rigid->pos) + dir;
		impact = rigid->pos - stat->pos;

		sf::Vector2f othervalrem = this->mult(impact, (this->dotpro(stat->velocity, impact) / (length * length)));
		sf::Vector2f thisvalrem = this->mult(impact, (this->dotpro(rigid->velocity, impact) / (length * length)));
		rigid->velocity = rigid->velocity - thisvalrem;


		sf::Vector2f deltathis = this->mult((othervalrem - thisvalrem), this->coeffofrestitution) + thisvalrem;
		rigid->velocity = rigid->velocity + deltathis;



	}


	for (auto itr = this->rigidobjects.begin(); itr != this->rigidobjects.end(); itr++)
	{
		for (auto itr2 = next(itr); itr2 != this->rigidobjects.end(); itr2++)
		{
			sf::Vector2f impact = (*itr)->pos - (*itr2)->pos;
			double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));
			if (length < ((*itr)->radius + (*itr2)->radius))
			{
				//std::cout << "collided==============================================================================" << std::endl;
				//std::cout << "frame " << f << std::endl;

				double overlap = ((*itr)->radius + (*itr2)->radius) - length;
				sf::Vector2f dir = impact;
				dir = this->setmag(dir, (overlap / 2.0) * 0.8);

				//distance correction
				(*itr)->pos = ((*itr)->pos) + dir;
				(*itr2)->pos = ((*itr2)->pos) - dir;
				length = (*itr)->radius + (*itr2)->radius;
				sf::Vector2f temp = (*itr)->pos - (*itr2)->pos;
				impact = this->setmag(impact, length);


				//removing the velocity vector along the impact line for both the balls
				sf::Vector2f othervalrem = this->mult(impact, (this->dotpro((*itr2)->velocity, impact) / (length * length)));
				(*itr2)->velocity = (*itr2)->velocity - othervalrem;
				sf::Vector2f thisvalrem = this->mult(impact, (this->dotpro((*itr)->velocity, impact) / (length * length)));
				(*itr)->velocity = (*itr)->velocity - thisvalrem;



				sf::Vector2f totalmomentum = this->mult(thisvalrem, (*itr)->mass) + this->mult(othervalrem, (*itr2)->mass);
				double msum = (*itr)->mass + (*itr2)->mass;

				sf::Vector2f deltathis = totalmomentum + this->mult((othervalrem - thisvalrem), ((*itr2)->mass * this->coeffofrestitution));
				deltathis = this->mult(deltathis, (1.0 / msum));
				(*itr)->velocity = (*itr)->velocity + deltathis;

				sf::Vector2f deltaother = totalmomentum + this->mult((thisvalrem - othervalrem), ((*itr)->mass * this->coeffofrestitution));
				deltaother = this->mult(deltaother, (1.0 / msum));
				(*itr2)->velocity = (*itr2)->velocity + deltaother;


			}
		}
	}
}

double PhysicsEngine::getmag(sf::Vector2f vec)
{
	double length = std::sqrtf((vec.x * vec.x) + (vec.y * vec.y));
	return length;
}

sf::Vector2f PhysicsEngine::setmag(sf::Vector2f vec, double mag)
{
	double length = std::sqrtf((vec.x * vec.x) + (vec.y * vec.y));
	vec.x = (vec.x * mag) / length;
	vec.y = (vec.y * mag) / length;
	return vec;

}

double PhysicsEngine::dotpro(sf::Vector2f vec1, sf::Vector2f vec2)
{
	double ans = (vec1.x * vec2.x) + (vec1.y * vec2.y);
	return (ans);
	//return abs(ans);
}

sf::Vector2f PhysicsEngine::mult(sf::Vector2f vec, double factor)
{
	vec.x = vec.x * factor;
	vec.y = vec.y * factor;
	return vec;
}

void PhysicsEngine::testingengine()
{
	//std::cout << "inside engine " << std::endl;
}
