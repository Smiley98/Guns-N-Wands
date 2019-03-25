#pragma once
#include "Projectile.h"
class SlowShot :
	public Projectile
{
	friend Projectile;
public:
	SlowShot(const Point&, const float&, const float&, const float&, const float&, std::string);
	~SlowShot();

	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;
};

