#include "DefBoost.h"

DefBoost::DefBoost(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
	detonator.changeLength(1000000000);
	detonator.reset();
}

DefBoost::~DefBoost()
{
}

bool DefBoost::move() {
	if (detonator.ready())
		return false;
	return true;
}

bool DefBoost::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	defence = def + _damage;
	effectInd = 3;
	return true;

}