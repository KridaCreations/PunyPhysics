#include "../header/PhysicsWorld.h"


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

		
	}
}

void PhysicsWorld::calculatemovement(double delta)
{
	
	for (auto& it : PhysicsWorld::getInstance()->bodies)
	{
		if (it->bodyType == RigidBody::Static)
		{
			it->velocity = it->velocity + this->mult(it->acceleration, delta);
		}
		else
		{
			it->velocity = it->velocity + this->mult(it->acceleration + this->gravity, delta);
		}
		if (it->shapetype = RigidBody::Circle)
		{
			RigidCircleShape* circlebody = static_cast<RigidCircleShape*>(it);
			sf::Vector2f newpos = circlebody->shape.getPosition() + this->mult(it->velocity, delta);
			circlebody->shape.setPosition(newpos);
		}
		else if (it->shapetype = RigidBody::Rectangle)
		{
			RigidRectangleShape* circlebody = static_cast<RigidRectangleShape*>(it);
			sf::Vector2f newpos = circlebody->shape.getPosition() + this->mult(it->velocity, delta);
			circlebody->shape.setPosition(newpos);
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
	//return abs(ans);
}

sf::Vector2f PhysicsWorld::mult(sf::Vector2f vec, double factor)
{
	vec.x = vec.x * factor;
	vec.y = vec.y * factor;
	return vec;
}