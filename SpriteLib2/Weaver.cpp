#include "Weaver.h"

Weaver::Weaver()
{
}


Weaver::~Weaver()
{
}

// projectiles
Projectile* Weaver::shootBasic(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0], imgArray[0]);
}
Projectile* Weaver::shootSpecial_1(Point pos, float rot) {
	return new SpeedBoost(pos, rot, projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]); // self speed boost
}
Projectile* Weaver::shootSpecial_2(Point pos, float rot) {
	return new Explosion(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]); // smoke bomb with no damage and no knockback
}
Projectile* Weaver::shootSpecial_3(Point pos, float rot) {
	return new PoisonShot(pos, rot, projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]); // poison shot
}
Projectile* Weaver::shootSpecial_4(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]); // rapid fire small projectile
}

// getters
cooldown& Weaver::getCool(const int& index)
{
	return this->cooldowns[index];
}
cooldown& Weaver::getDelay(const int& index)
{
	return this->delays[index];
}
cooldown& Weaver::getGlobalCool()
{
	return this->globalCooldown;
}
float& Weaver::getDefence() {
	return this->defence;
}
float& Weaver::getDef() {
	return this->def;
}
float& Weaver::getSpeed() {
	return this->moveSpeed;
}
float& Weaver::getSpd() {
	return this->spd;
}

float& Weaver::projectileRadius(const int& index)
{
	return this->projectileRadii[index];
}