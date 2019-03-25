#pragma once
#include "Character.h"
#include "SpeedBoost.h"
#include "Thrown.h"
#include "Phasing.h"

class Recon :
	public Character
{
public:
	Recon();
	~Recon();
	Teams team = Teams::Militians;
	cooldown globalCooldown;
#pragma region Data
	float defence = 15;
	float moveSpeed = 7.0f;
	float def = 15; // initialized as exact same as defence
	float spd = 7.0f; // initialized as exact same as moveSpeed
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
		"Spells/BloodOrb",
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

