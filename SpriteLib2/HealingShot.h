#pragma once
#include "Projectile.h"
class HealingShot :
	public Projectile
{
	friend Projectile;
public:
	HealingShot(const Point&, const float&, const float&, const float&, const float&, std::string);
	~HealingShot();

	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;
};

