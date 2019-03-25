#include "Explosion.h"



Explosion::Explosion(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
	detonator.changeLength(1000000000);
	detonator.reset();
}


Explosion::~Explosion()
{
}

bool Explosion::move() {
	if (detonator.ready())
		return false;
	return true;
}

bool Explosion::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	health -= _damage * (defence / 100);
	return false;
}