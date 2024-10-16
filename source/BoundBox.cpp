#include "../header/BoundBox.h"


BoundBox::BoundBox(int width, int height, int x, int y)
{
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;

	this->reset_corners();
}

void BoundBox::reset_corners()
{
	this->tl = sf::Vector2f(this->x - (this->width / 2), this->y - (this->height / 2));
	this->tr = sf::Vector2f(this->x + (this->width / 2), this->y - (this->height / 2));
	this->bl = sf::Vector2f(this->x - (this->width / 2), this->y + (this->height / 2));
	this->br = sf::Vector2f(this->x + (this->width / 2), this->y + (this->height / 2));
	std::cout << "pre " << (this->x) << " " << (this->width / 2) << " " << (this->y) << " " << (this->height / 2) << std::endl;
	top = this->y - (this->height / 2);
	bottom = this->y + (this->height / 2);
	left = this->x - (this->width / 2);
	right = this->x + (this->width / 2);
	std::cout <<"reseting corners " << top << " " << bottom << " " << left << " " << right << std::endl;
}

void BoundBox::setnew(int width, int height, int x, int y)
{
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;

	this->reset_corners();
}