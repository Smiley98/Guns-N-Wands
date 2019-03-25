#include "Line.h"
#include "Point.h"
#include "spritelib\shapes.h"
#include <iostream>

Line::Line()
{
}

Line::Line(const Point& start, const Point& end)
{
	_start = start;
	_end = end;
}

bool Line::intersect(Line otherLine, Point &outPoint)
{
	Point r((_end._x - _start._x), (_end._y - _start._y));
	Point s((otherLine._end._x - otherLine._start._x), (otherLine._end._y - otherLine._start._y));

	float d = r.cross(s);

	float u = (otherLine._start.cross(r) - _start.cross(r)) / d;	//u and t are scalar values of parameterics
	float t = (otherLine._start.cross(s) - _start.cross(s)) / d;

	if ((0 <= u) && (u <= 1) && (0 <= t) && (t <= 1))
	{
		outPoint = Point((r._x * t) + _start._x, (r._y * t) + _start._y);
		return true;
	}
	return false;
}

bool Line::intersect(Line otherLine)
{
	Point r((_end._x - _start._x), (_end._y - _start._y));
	Point s((otherLine._end._x - otherLine._start._x), (otherLine._end._y - otherLine._start._y));

	float d = r.cross(s);

	float u = (otherLine._start.cross(r) - _start.cross(r)) / d;
	float t = (otherLine._start.cross(s) - _start.cross(s)) / d;

	if ((0 <= u) && (u <= 1) && (0 <= t) && (t <= 1))
		return true;
	return false;
}

void Line::drawColour()
{
	spritelib::Shapes::set_color(c.x, c.y, c.z);
	spritelib::Shapes::draw_line(_start._x, _start._y, _end._x, _end._y);
}
