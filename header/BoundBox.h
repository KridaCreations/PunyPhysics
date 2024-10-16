#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

class BoundBox
{
public:
	int width, height;
	int x, y;
	sf::Vector2f tl;
	sf::Vector2f tr;
	sf::Vector2f bl;
	sf::Vector2f br;
	double top, bottom, left, right;

	void reset_corners();
	void setnew(int width, int height, int x, int y);

	BoundBox(int width, int height, int x, int y);

	BoundBox() = delete;
};

