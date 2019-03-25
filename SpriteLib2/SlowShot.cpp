#include "SlowShot.h"



SlowShot::SlowShot(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
}


SlowShot::~SlowShot()
{
}

bool SlowShot::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	speed = spd - _damage;
	effectInd = 4;
	return true;
}