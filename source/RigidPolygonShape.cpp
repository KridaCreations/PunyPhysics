#include "../header/RigidPolygonShape.h"

RigidPolygonShape::RigidPolygonShape(pum::vector2d pos, pum::vector2d velocity, pum::vector2d acceleration, std::vector<pum::vector2d> points, double mass, type bodytype, double coefficientOfStaticFriction, double coefficientOfKineticFriction)
	:RigidBody(mass, velocity, acceleration, bodytype, coefficientOfStaticFriction, coefficientOfKineticFriction)
{
	this->position = pos;
	int sides = points.size();
	this->points.resize(sides);
	this->relativePointPosition.resize(sides);
	int maxlen = LLONG_MIN;
	for (int i = 0; i < sides; i++)
	{
		this->points[i] = points[i];
		this->relativePointPosition[i] = points[i] - pos;
		maxlen = std::max((points[i] - points[(i + 1) % sides]).length(), (double)maxlen);
	}

	this->momentOfInertia = (mass * (maxlen * maxlen)) / 12.0;
	this->shapetype = RigidBody::shapetypes::Polygon;
	this->angle = 0;

}

void RigidPolygonShape::setrotation(double angle)
{
	this->rotate(-1 * this->angle);
	this->rotate(angle);
	this->angle = angle;
}

void RigidPolygonShape::setpositon(pum::vector2d pos)
{
	for (int point = 0; point < this->points.size(); point++)
	{
		this->points[point] = this->points[point] + (pos - this->position);
	}
	this->position = pos;
}

void RigidPolygonShape::translate(pum::vector2d pos)
{
	this->position = this->position + pos;//changing the position
	pum::vector2d newpos = this->position;
	for (int i = 0; i < this->points.size(); i++)
	{
		this->points[i] = this->relativePointPosition[i] + newpos;
	}


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

void RigidPolygonShape::rotate(double angle)
{
	this->angle = this->angle + angle;//changing the angle

	pum::vector2d newpos = this->position;
	for (int i = 0; i < this->points.size(); i++)
	{
		this->points[i] = this->relativePointPosition[i] + newpos;
	}

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
