#pragma once
#include "Projectile.h"
class SpeedBoost :
	public Projectile
{
	friend Projectile;
public:
	SpeedBoost(const Point&, const float&, const float&, const float&, const float&, std::string);
	~SpeedBoost();

	virtual bool move() override;
	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;

private:
	cooldown detonator;
};

