#pragma once
#include "Projectile.h"
class DefBoost :
	public Projectile
{
	friend Projectile;
public:
	DefBoost(const Point&, const float&, const float&, const float&, const float&, std::string);
	~DefBoost();

	virtual bool move() override;
	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;

private:
	cooldown detonator;
};

