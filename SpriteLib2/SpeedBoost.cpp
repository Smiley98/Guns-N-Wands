#include "SpeedBoost.h"

SpeedBoost::SpeedBoost(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
	detonator.changeLength(1000000000);
	detonator.reset();
}


SpeedBoost::~SpeedBoost()
{
}

bool SpeedBoost::move() {
	if (detonator.ready())
		return false;
	return true;
}

bool SpeedBoost::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	speed = spd + _damage;
	effectInd = 4;
	return true;
}