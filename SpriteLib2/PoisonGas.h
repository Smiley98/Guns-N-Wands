#pragma once
#include "Projectile.h"
class PoisonGas :
	public Projectile
{
	friend Projectile;
public:
	PoisonGas(const Point&, const float&, const float&, const float&, const float&, std::string);
	~PoisonGas();

	virtual bool move() override;
	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;

private:
	cooldown detonator;
};

