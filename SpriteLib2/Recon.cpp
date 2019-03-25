#include "Recon.h"

Recon::Recon()
{
}

Recon::~Recon()
{
}

// projectiles
Projectile* Recon::shootBasic(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0], imgArray[0]);
}
Projectile* Recon::shootSpecial_1(Point pos, float rot) {
	return new SpeedBoost(pos, rot, projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]); // speed boost
}
Projectile* Recon::shootSpecial_2(Point pos, float rot) {
	return new Thrown(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]); // smoke grenade
}
Projectile* Recon::shootSpecial_3(Point pos, float rot) {
	return new Phasing(pos, rot, projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]); // laser shot
}
Projectile* Recon::shootSpecial_4(Point pos, float rot) {
	return new Thrown(pos, rot,  projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]); // normal grenade
}

// getters
cooldown& Recon::getCool(const int& index)
{
	return this->cooldowns[index];
}
cooldown& Recon::getDelay(const int& index)
{
	return this->delays[index];
}
cooldown& Recon::getGlobalCool()
{
	return this->globalCooldown;
}
float& Recon::getDefence() {
	return this->defence;
}
float& Recon::getDef() {
	return this->def;
}
float& Recon::getSpeed() {
	return this->moveSpeed;
}
float& Recon::getSpd() {
	return this->spd;
}

float& Recon::projectileRadius(const int& index)
{
	return this->projectileRadii[index];
}
