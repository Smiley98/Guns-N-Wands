#pragma once
#include "Line.h"
class Circle
{
public:
	Circle(Point, float);
	Circle();
	void draw();
	bool intersect(Line); 
	bool intersect(const Circle&);
	void move(float, float);
	float _rad;
	Point _centre;
};