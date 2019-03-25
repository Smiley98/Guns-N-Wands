#pragma once
#include "Projectile.h"
#include "cooldown.h"

class Player;
class Character
{
	friend Player;
protected:
	Character();
	~Character();

	//radius and health are the only two properties that do not get modified upon inheritance (only two properties we don't need to write a function to return a reference to)
	float health = 100.0f;
	float radius = 15.0f;

	//EVERYT character stat must have a function that returns a reference to it
	float defence;
	float moveSpeed;
	float def;
	float spd;
	cooldown globalCooldown;
	cooldown delays[5];
	cooldown cooldowns[5];

	//EVERY Projectile does not need a function that returns a reference to it
	std::string imgArray[5];
	float spellDamages[5];
	float projectileSpeeds[5];
	float projectileRadii[5];

	virtual Projectile* shootBasic(Point, float) = 0;
	virtual Projectile* shootSpecial_1(Point, float) = 0;
	virtual Projectile* shootSpecial_2(Point, float) = 0;
	virtual Projectile* shootSpecial_3(Point, float) = 0;
	virtual Projectile* shootSpecial_4(Point, float) = 0;

	virtual cooldown& getCool(const int&) = 0;
	virtual cooldown& getDelay(const int&) = 0;
	virtual cooldown& getGlobalCool() = 0;
	virtual float& getDefence() = 0;
	virtual float& getDef() = 0;
	virtual float& getSpeed() = 0;
	virtual float& getSpd() = 0;

	virtual float& projectileRadius(const int&) = 0;

	//IMPORTANT: unlike the rest of the cooldowns, effect durations can stay inside
	//Character because all effects can be applied to any character
	const static unsigned int numEffects;
	cooldown effectDurations[15] = {
		cooldown(cooldown::toMilliseconds(0)),		//none
		cooldown(cooldown::toMilliseconds(1000)),	//poison
		cooldown(cooldown::toMilliseconds(3000)),	//heal
		cooldown(cooldown::toMilliseconds(1000)),	//defChange
		cooldown(cooldown::toMilliseconds(7000))	//spdChange
	};
};