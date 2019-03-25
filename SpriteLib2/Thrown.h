#pragma once
#include "Projectile.h"
class Thrown :
	public Projectile
{
	friend Projectile;
public:
	Thrown(const Point&, const float&, const float&, const float&, const float&, std::string);
	~Thrown();

	virtual bool move() override;
	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&) override;
	
private:
	cooldown detonator;
	cooldown explosion_timer;
	int fuse;
	bool exp_phase;
};

