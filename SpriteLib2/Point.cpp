#include "Point.h"
#include <cmath>

Point::Point(float x, float y)
{
	_x = x;
	_y = y;
}

float Point::cross(Point point2)
{
	return ((_x * point2._y) - (_y * point2._x));
}

float Point::mag()
{
	return sqrt((pow(_x, 2) + pow(_y, 2)));
}

float Point::operator*(const Point otherPoint)
{
	return ((_x * otherPoint._x) + (_y * otherPoint._y));
} 

Point Point::operator*(float n)
{
	return Point(_x * n, _y * n);
}

Point Point::operator/=(float divisor)
{
	return Point(
		_x /= divisor,
		_y /= divisor
	);
}

Point Point::proj(Point b)
{
	float top = *this * b;
	float bot = pow(b.mag(), 2);

	return b * (top / bot);

}