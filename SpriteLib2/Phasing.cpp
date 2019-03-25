#include "Phasing.h"

Phasing::Phasing(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
}

Phasing::~Phasing()
{
}

bool Phasing::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	health -= (_damage * (defence / 100.0f));
	return false; // only change is projectile isn't destroyed
}