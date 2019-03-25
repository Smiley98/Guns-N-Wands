#include "HealingShot.h"

HealingShot::HealingShot(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
}

HealingShot::~HealingShot()
{
}
							//you could make this work by passing in an address of an attributes
bool HealingShot::projectileEffect(float& HealingShotth, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	HealingShotth += _damage;
	return true;
}