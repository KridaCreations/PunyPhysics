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
		(this->objects).insert(obj);
		return lastindex;
	}
	else
	{
		(this->indexes).push_back(obj);
		(this->objects).insert(obj);
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
	//calculating velocity of all objects
	for (auto& it : this->objects)
	{
		it->veladd.x = it->veladd.x + (it->acceleration.x * delta);
		it->veladd.y = it->veladd.y + (it->acceleration.y * delta);
		it->veladd.x = it->veladd.x + (this->gravity.x * delta);
		it->veladd.y = it->veladd.y + (this->gravity.y * delta);
	}

	//calculating position of all objects
	for (auto& it : this->objects)
	{
		it->posadd.x = it->posadd.x + (it->velocity.x * delta);
		it->posadd.y = it->posadd.y + (it->velocity.y * delta);
		//std::cout << (it->velocity.y) << " " << delta << " " << (double)((double)it->velocity.y * delta) << std::endl;
	}

	

	//adding the position to newposition
	for (auto& it : this->objects)
	{
		//std::cout << "prev pos " << (it->pos.x) << " " << (it->pos.y) << std::endl;
		it->pos.x = it->pos.x + it->posadd.x;
		it->pos.y = it->pos.y + it->posadd.y;
		//std::cout <<"possadd " << (it->posadd.x) << " " << (it->posadd.y) << std::endl;
		//std::cout << "pos " << (it->pos.x) << " " << (it->pos.y) << std::endl;
		it->posadd.x = 0, it->posadd.y = 0;

		it->velocity.x = it->velocity.x + it->veladd.x;
		it->velocity.y = it->velocity.y + it->veladd.y;
		//std::cout << (it->velocity.x) << " " << (it->velocity.y) << std::endl;
		it->veladd.x = 0, it->veladd.y = 0;
	}
	
	
	this->solveCollisionCumulative();
	//this->solveCollisionDirect();

}

void PhysicsEngine::solveCollisionCumulative()
{

	double resolutionfactor = 0.8;
	int maxiter = 15;
	// resolving collision

	//std::cout << "checking collision " << std::endl;

	for (int i = 0; i < maxiter; i++)
	{
		for (auto& it : this->objects)
		{
			//checking for collision with the bounding box
			if ((it->pos.x + it->radius) > bh->right)
			{
				//std::cout << "collidec right" << std::endl;
				it->velocity.x = -1 * abs(it->velocity.x);// (-(1 + this->coeffofrestitution) * abs(it->velocity.x));
				double diff = it->pos.x + it->radius - bh->right;
				it->pos.x = it->pos.x - (diff * resolutionfactor);
			}
			if ((it->pos.x - it->radius) < (bh->left))
			{
				//std::cout << "collidec left" << std::endl;
				it->velocity.x = 1 * abs(it->velocity.x);// (1 + this->coeffofrestitution)* abs(it->velocity.x);
				double diff = bh->left - (it->pos.x - it->radius);
				it->pos.x = it->pos.x + (diff * resolutionfactor);
			}
			if ((it->pos.y + it->radius) > (bh->bottom))
			{
				//std::cout << "collidec bottom" << std::endl;
				it->velocity.y = -1 * abs(it->velocity.y); //(-(1 + this->coeffofrestitution) * abs(it->velocity.y));
				double diff = (it->pos.y + it->radius) - (bh->bottom);
				it->pos.y = it->pos.y - (diff * resolutionfactor);
			}
			if ((it->pos.y - it->radius) < (bh->top))
			{
				//std::cout << "collidec top" << std::endl;
				it->velocity.y = 1 * abs(it->velocity.y);// (1 + this->coeffofrestitution)* abs(it->velocity.y);
				double diff = bh->top - (it->pos.y - it->radius);
				it->pos.y = it->pos.y + (diff * resolutionfactor);
			}
		}
		
		/*for (auto& it : this->objects)
		{
			it->velocity.x = it->velocity.x + it->veladd.x;
			it->velocity.y = it->velocity.y + it->veladd.y;
			it->veladd.x = 0, it->veladd.y = 0;
		}*/

		//continue;
		//for other bodies
		std::vector<std::pair<RigidCircle*, RigidCircle*>>collided;
		for (auto itr = this->objects.begin(); itr != this->objects.end(); itr++)
		{
			for (auto itr2 = next(itr); itr2 != this->objects.end(); itr2++)
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
			sf::Vector2f temp = first->pos - second->pos;
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

	for (auto& it : this->objects)
	{
		//checking for collision with the bounding box
		if ((it->pos.x + it->radius) > bh->right)
		{
			it->velocity.x = -1 * abs(it->velocity.x);// (-(1 + this->coeffofrestitution) * abs(it->velocity.x));
			it->pos.x = bh->right - (it->radius);
		}
		if ((it->pos.x - it->radius) < (bh->left))
		{
			it->velocity.x = 1 * abs(it->velocity.x);// (1 + this->coeffofrestitution)* abs(it->velocity.x);
			it->pos.x = bh->left + (it->radius);
		}
		if ((it->pos.y + it->radius) > (bh->bottom))
		{
			it->velocity.y = -1 * abs(it->velocity.y); //(-(1 + this->coeffofrestitution) * abs(it->velocity.y));
			it->pos.y = bh->bottom - (it->radius);
		}
		if ((it->pos.y - it->radius) < (bh->top))
		{
			it->velocity.y = 1 * abs(it->velocity.y);// (1 + this->coeffofrestitution)* abs(it->velocity.y);
			it->pos.y = bh->top + (it->radius);
		}
	}

	for (auto itr = this->objects.begin(); itr != this->objects.end(); itr++)
	{
		for (auto itr2 = next(itr); itr2 != this->objects.end(); itr2++)
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
