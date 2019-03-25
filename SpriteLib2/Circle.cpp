#include "Circle.h"
#include "spritelib\shapes.h"
#include <iostream>

Circle::Circle(Point centre, float radius)
{
	_centre = centre;
	_rad = radius;
}

Circle::Circle()
{
	_centre = Point{ 0.0f, 0.0f };
	_rad = 10.0f;
}

void Circle::draw()
{
	spritelib::Shapes::draw_circle(false, _rad, _centre._x, _centre._y);
}

void Circle::move(float x, float y)
{
	_centre._x += x;
	_centre._y += y;
}

bool Circle::intersect(Line line)
{ 
	//Vector from line.start -> line.end
	Point a_b(line._end._x - line._start._x, line._end._y - line._start._y);

	//Vector from line.start -> circle centre
	Point a_c(_centre._x - line._start._x, _centre._y - line._start._y);
	Point a_d = a_c.proj(a_b);

	//Vector from projection -> circle centre
	Point d_c(a_c._x - a_d._x ,a_c._y - a_d._y);

	//Vector from line.end -> line.start
	Point b_a(line._start._x - line._end._x, line._start._y - line._end._y);

	//Vector from line.end -> circle centre
	Point b_c(_centre._x - line._end._x, _centre._y - line._end._y);
	Point b_d = b_c.proj(b_a);

	//If the projections are in the same direction as the line, and the circle radius <= the distance to the line, there is an intersection
	if (((a_b * a_d) > 0) && ((b_a * b_d) > 0))
	{
		if (d_c.mag() <= _rad)
			return true;
	}

	return false;
} 

bool Circle::intersect(const Circle& circle)
{
	//get the distance of the line between circles and check if it is less than the distance of the two radii
	float dy = _centre._y - circle._centre._y;
	float dx = _centre._x - circle._centre._x;
	if (std::sqrt(dx * dx + dy * dy) < _rad + circle._rad) return true;
	return false;
}
