#pragma once
#include "Projectile.h"
class PoisonShot :
	public Projectile
{
public:
	PoisonShot(const Point&, const float&, const float&, const float&, const float&, std::string);
	~PoisonShot();

	virtual bool move() override;
	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;
};

