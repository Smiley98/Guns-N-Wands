#pragma once
class Point
{
public:
	Point(float = 0, float = 0);
	float cross(Point);
	Point proj(const Point);
	float operator*(const Point);
	Point operator*(float);
	Point operator/=(float);
	float mag();
	float _x, _y;
};