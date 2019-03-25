#pragma once
#include "Projectile.h"
class Phasing :
	public Projectile
{
	friend Projectile;
public:
	Phasing(const Point&, const float&, const float&, const float&, const float&, std::string);
	~Phasing();

	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;
};

