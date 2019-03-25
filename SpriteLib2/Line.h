#pragma once
#include "Point.h"
#include "spritelib\vector3.h"
#include <cmath>

typedef spritelib::math::Vector3 vec3;

class RayEngine;
class Level;
class Projectile;
class Circle;

class Line
{
	friend RayEngine;
	friend Level;
	friend Projectile;
	friend Circle;
public:
	Line();
	Line(const Point&, const Point&);
	bool intersect(Line, Point&);
	bool intersect(Line);
	void drawColour();
	inline float calcLength()	//must define the inline function in the header to prevent a linker error
	{
		float deltaX = _end._x - _start._x;
		float deltaY = _end._y - _start._y;
		return std::sqrt(deltaX*deltaX + deltaY*deltaY);
	}
private:
	Point _start, _end;
	vec3 c;
	float w;
	float h;
	unsigned int number_;
	char texID[16];
};