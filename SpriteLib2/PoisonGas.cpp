#include "PoisonGas.h"

PoisonGas::PoisonGas(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
	detonator.changeLength(1000000000);
	detonator.reset();
}

PoisonGas::~PoisonGas()
{
}

bool PoisonGas::move() {
	Circle updatedBody = Circle(Point(body._centre._x + _cosRot * _speed, body._centre._y + _sinRot * _speed), body._rad);
	if (collideLine(updatedBody) || detonator.ready())
		return false;
	body = updatedBody;
	return true;
}

bool PoisonGas::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	effectInd = 1;
	return false;
}