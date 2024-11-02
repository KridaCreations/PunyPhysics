#include "../header/RigidRectangelShape.h"
#include <iostream>
#include "../header/PhysicsWorld.h"

RigidRectangleShape::RigidRectangleShape(pum::vector2d pos, pum::vector2d velocity, pum::vector2d acceleration,double width,double length,double mass,type bodytype, double coefficientOfStaticFriction , double coefficientOfKineticFriction)
	:RigidBody(mass,velocity,acceleration, bodytype, coefficientOfStaticFriction, coefficientOfKineticFriction)
{

	this->length = length;
	this->width = width;
	this->momentOfInertia = (mass * ((width * width) + (length * length))) / 12.0;
	this->shapetype = RigidBody::shapetypes::Rectangle;
	
	this->position = pos;
	this->points[0] = pum::vector2d(pos.x - (width / 2.0), pos.y - (length / 2.0));
	this->points[1] = pum::vector2d(pos.x + (width / 2.0), pos.y - (length / 2.0));
	this->points[2] = pum::vector2d(pos.x + (width / 2.0), pos.y + (length / 2.0));
	this->points[3] = pum::vector2d(pos.x - (width / 2.0), pos.y + (length / 2.0));
	
	this->angle = 0;

}

void RigidRectangleShape::rotate(double angle)
{
	this->angle = this->angle + angle;

	pum::vector2d newpos = this->position;
	this->points[0] = pum::vector2d(newpos.x - (width / 2.0), newpos.y - (length / 2.0));
	this->points[1] = pum::vector2d(newpos.x + (width / 2.0), newpos.y - (length / 2.0));
	this->points[2] = pum::vector2d(newpos.x + (width / 2.0), newpos.y + (length / 2.0));
	this->points[3] = pum::vector2d(newpos.x - (width / 2.0), newpos.y + (length / 2.0));


	//again rotating by the body angle
	double rad = this->deg2rad(this->angle);
	for (int point = 0; point < this->points.size(); point++)
	{
		this->points[point] = this->points[point] - this->position;
		pum::vector2d temp = this->points[point];
		this->points[point] = pum::vector2d(((temp.x * cos(rad)) - (temp.y * sin(rad))), ((temp.x * sin(rad)) + (temp.y * cos(rad))));
		this->points[point] = this->points[point] + this->position;
	}
	//std::cout << std::endl;
}

void RigidRectangleShape::translate(pum::vector2d pos)
{
	this->position = this->position + pos;//changing the position
	/*if (bodyType == RigidBody::Rigid)
	{
		std::cout << this->position.x << " " << this->position.y << std::endl;
	}
	std::cout << std::endl;*/
	//freshly deciding the body position
	pum::vector2d newpos = this->position;
	this->points[0] = pum::vector2d(newpos.x - (width / 2.0), newpos.y - (length / 2.0));
	this->points[1] = pum::vector2d(newpos.x + (width / 2.0), newpos.y - (length / 2.0));
	this->points[2] = pum::vector2d(newpos.x + (width / 2.0), newpos.y + (length / 2.0));
	this->points[3] = pum::vector2d(newpos.x - (width / 2.0), newpos.y + (length / 2.0));


	//again rotating by the body angle
	double rad = this->deg2rad(this->angle);
	for (int point = 0; point < this->points.size(); point++)
	{
		this->points[point] = this->points[point] - this->position;
		pum::vector2d temp = this->points[point];
		this->points[point] = pum::vector2d(((temp.x * cos(rad)) - (temp.y * sin(rad))), ((temp.x * sin(rad)) + (temp.y * cos(rad))));
		this->points[point] = this->points[point] + this->position;
	}
}

void RigidRectangleShape::setpositon(pum::vector2d pos)
{
	for (int point = 0; point < this->points.size(); point++)
	{
		this->points[point] = this->points[point] + (pos - this->position);
	}
	this->position = pos;
}

void RigidRectangleShape::setrotation(double angle)
{
	this->rotate(-1 * this->angle);
	this->rotate(angle);
	this->angle = angle;
}