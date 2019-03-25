#pragma once
#include "Character.h"
class Destroyer :
	public Character
{
public:
	Destroyer();
	~Destroyer();
#pragma region Character Stats
	float defence = 25;
	float moveSpeed = 6.0f;
	float def = 25;		// initialized as exact same as defence
	float spd = 6.0f;	// initialized as exact same as moveSpeed
	cooldown globalCooldown;
	cooldown delays[5] = {	//penalty for switching spells
		cooldown(cooldown::toMilliseconds(1000)),	//basic attack
		cooldown(cooldown::toMilliseconds(250)),
		cooldown(cooldown::toMilliseconds(0)),
		cooldown(cooldown::toMilliseconds(500)),
		cooldown(cooldown::toMilliseconds(2500))
	};
	cooldown cooldowns[5] = {	//spell cooldowns
		cooldown(cooldown::toMilliseconds(100)),	//basic attack
		cooldown(cooldown::toMilliseconds(250)),
		cooldown(cooldown::toMilliseconds(500)),
		cooldown(cooldown::toMilliseconds(750)),
		cooldown(cooldown::toMilliseconds(1000))
	};
#pragma endregion
#pragma region Projectile Stats
	float spellDamages[5] = {
		5.0f,
		15.0f,
		10.0f,
		20.0f,
		50.0f
	};
	//*IMPORTANT: projectile speed cannot exceed twice its radii otherwise tunneling may occur
	float projectileSpeeds[5] = {
		8.0f,
		6.0f,
		10.0f,
		6.0f,
		2.0f
	};
	float projectileRadii[5] = {
		10.0f,
		12.5f,
		5.0f,
		7.5f,
		50.0f
	};
	std::string imgArray[5] = {
		"cannonball",
		"Spells/WaterOrb",
		"Spells/FireOrb",
		"Spells/LightningOrb",
		"Spells/IceOrb"
	};
#pragma endregion
	virtual Projectile* shootBasic(Point, float) override;
	virtual Projectile* shootSpecial_1(Point, float) override;
	virtual Projectile* shootSpecial_2(Point, float) override;
	virtual Projectile* shootSpecial_3(Point, float) override;
	virtual Projectile* shootSpecial_4(Point, float) override;
	
	virtual cooldown& getCool(const int&) override;
	virtual cooldown& getDelay(const int&) override;
	virtual cooldown& getGlobalCool() override;
	virtual float& getDefence() override;
	virtual float& getDef() override;
	virtual float& getSpeed() override;
	virtual float& getSpd() override;

	virtual float& projectileRadius(const int&) override;
};

