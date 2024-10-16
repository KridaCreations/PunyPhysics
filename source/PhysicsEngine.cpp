#include "../header/PhysicsEngine.h"
#include <iostream>

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
		it->pos.x = it->pos.x + it->posadd.x;
		it->pos.y = it->pos.y + it->posadd.y;
		it->posadd.x = 0, it->posadd.y = 0;

		it->velocity.x = it->velocity.x + it->veladd.x;
		it->velocity.y = it->velocity.y + it->veladd.y;
		it->veladd.x = 0, it->veladd.y = 0;
	}
	// resolving collision
	for (auto& it : this->objects)
	{
		//checking for collision with the bounding box
		if ((it->pos.x + it->radius) > bh->right)
		{
			it->veladd.x = (-(1 + this->coeffofrestitution) * abs(it->velocity.x));
			it->pos.x = bh->right - it->radius;
		}
		if ((it->pos.x - it->radius) < (bh->left))
		{
			it->veladd.x = (1 + this->coeffofrestitution) * abs(it->velocity.x);
			it->pos.x = bh->left + it->radius;
		}
		if ((it->pos.y + it->radius) > (bh->bottom))
		{
			it->veladd.y = (-(1 + this->coeffofrestitution) * abs(it->velocity.y));
			it->pos.y = bh->bottom - it->radius;
		}
		if ((it->pos.y - it->radius) < (bh->top))
		{
			it->veladd.y = (1 + this->coeffofrestitution) * abs(it->velocity.y);
			it->pos.y = bh->top + it->radius;
		}
	}

	std::cout << "here " << std::endl;
	for (auto itr = this->objects.begin();itr != this->objects.end();itr++)
	{
		for (auto itr2 = next(itr); itr2 != this->objects.end(); itr2++)
		{
			sf::Vector2f impact = (*itr)->pos - (*itr2)->pos;
			double length = std::sqrtf((impact.x * impact.x) + (impact.y * impact.y));
			if (length < ((*itr)->radius + (*itr2)->radius))
			{
				std::cout << "check" << std::endl;
				//balls collided
				double overlap = ((*itr)->radius + (*itr2)->radius) - length;
				sf::Vector2f dir = impact;
				dir = this->setmag(dir, overlap / 2.0);

				//distance correction
				(*itr)->pos = ((*itr)->pos) + dir;
				(*itr2)->pos = ((*itr2)->pos) - dir;
				length = (*itr)->radius + (*itr2)->radius;
				//std::cout<<"len " << length << std::endl;
				sf::Vector2f temp = (*itr)->pos - (*itr2)->pos;
				double templen = std::sqrt((temp.x * temp.x) + (temp.y * temp.y));
				//std::cout << templen << std::endl;

				impact = this->setmag(impact, length);

				double msum = (*itr)->mass + (*itr2)->mass;
				sf::Vector2f vdiff = (*itr)->velocity - (*itr2)->velocity;
				double num = this->dotpro(vdiff, impact);
				double den = msum * length * length;
				//particle A
				sf::Vector2f deltaVA = impact;
				deltaVA = this->mult(deltaVA,((2 * ((*itr2)->mass) * num) / den));
				(*itr)->veladd = (*itr)->veladd + deltaVA;

				//particle B
				sf::Vector2f deltaVB = impact;
				deltaVB = this->mult(deltaVB, ((-2 * (*itr)->mass * num) / den));
				(*itr2)->veladd = (*itr2)->veladd + deltaVB;



				//let mSum = this.mass + other.mass;
				//let vDiff = p5.Vector.sub(other.velocity, this.velocity);
				//// Particle A (this)
				//let num = vDiff.dot(impactVector);
				//let den = mSum * d * d;
				//let deltaVA = impactVector.copy();
				//deltaVA.mult((2 * other.mass * num) * 0.1 / den);
				//this.velocity.add(deltaVA);
				//// Particle B (other)
				//let deltaVB = impactVector.copy();
				//deltaVB.mult((-2 * this.mass * num) * 0.1 / den);
				//other.velocity.add(deltaVB);



			}
		}
	}

	for (auto& it : this->objects)
	{
		it->velocity.x = it->velocity.x + it->veladd.x;
		it->velocity.y = it->velocity.y + it->veladd.y;
		it->veladd.x = 0, it->veladd.y = 0;
	}
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
	double ans = (vec1.x * vec2.x) + (vec1.x * vec2.y);
	return ans;
}

sf::Vector2f PhysicsEngine::mult(sf::Vector2f vec, double factor)
{
	vec.x = vec.x * factor;
	vec.y = vec.y * factor;
	return vec;
}

void PhysicsEngine::testingengine()
{
	std::cout << "inside engine " << std::endl;
}
