#include "Medic.h"

Medic::Medic()
{
	srand((unsigned)time(0));
}


Medic::~Medic()
{
}

// projectiles
Projectile* Medic::shootBasic(Point pos, float rot) {
	return new HealingShot(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0], imgArray[0]);
}
Projectile* Medic::shootSpecial_1(Point pos, float rot) {
	return new SlowShot(pos, rot, projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]); // make enemies slow
}
Projectile* Medic::shootSpecial_2(Point pos, float rot) {
	return new DefBoost(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]); // defence self buff
}
Projectile* Medic::shootSpecial_3(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]); // shotgun
}
Projectile* Medic::shootSpecial_4(Point pos, float rot) {
	return new Thrown(pos, rot, projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]); // normal grenade
}

// getters
cooldown& Medic::getCool(const int& index)
{
	return this->cooldowns[index];
}
cooldown& Medic::getDelay(const int& index)
{
	return this->delays[index];
}
cooldown& Medic::getGlobalCool()
{
	return this->globalCooldown;
}
float& Medic::getDefence() {
	return this->defence;
}
float& Medic::getDef() {
	return this->def;
}
float& Medic::getSpeed() {
	return this->moveSpeed;
}
float& Medic::getSpd() {
	return this->spd;
}

float& Medic::projectileRadius(const int& index)
{
	return this->projectileRadii[index];
}