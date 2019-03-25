#pragma once
#include "Projectile.h"
class Explosion :
	public Projectile
{
	friend Projectile;
public:
	Explosion(const Point&, const float&, const float&, const float&, const float&, std::string);
	~Explosion();

	virtual bool move() override;
	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;

private:
	cooldown detonator;
};

