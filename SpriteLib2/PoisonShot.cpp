#include "PoisonShot.h"



PoisonShot::PoisonShot(const Point&, const float&, const float&, const float&, const float&, std::string)
{
}


PoisonShot::~PoisonShot()
{
}

bool PoisonShot::move() {
	Circle updatedBody = Circle(Point(body._centre._x + _cosRot * _speed, body._centre._y + _sinRot * _speed), body._rad);
	if (collideLine(updatedBody))
		return false;
	body = updatedBody;
	return true;
}

bool PoisonShot::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	effectInd = 1;
	return true;
}
