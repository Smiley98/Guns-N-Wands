#include "Assault.h"

Assault::Assault()
{
}

Assault::~Assault()
{
}

// projectiles
Projectile* Assault::shootBasic(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0], imgArray[0]);
}
Projectile* Assault::shootSpecial_1(Point pos, float rot) {
	return new SpeedBoost(pos, rot, projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]); // speed boost
}
Projectile* Assault::shootSpecial_2(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]); // rapid fire minigun
}
Projectile* Assault::shootSpecial_3(Point pos, float rot) {
	return new Phasing(pos, rot, projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]); // laser shot
}
Projectile* Assault::shootSpecial_4(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]); // missile *NOTE* change to derived missile class later
}

// getters
cooldown& Assault::getCool(const int& index)
{
	return this->cooldowns[index];
}
cooldown& Assault::getDelay(const int& index)
{
	return this->delays[index];
}
cooldown& Assault::getGlobalCool()
{
	return this->globalCooldown;
}
float& Assault::getDefence() {
	return this->defence;
}
float& Assault::getDef() {
	return this->def;
}
float& Assault::getSpeed() {
	return this->moveSpeed;
}
float& Assault::getSpd() {
	return this->spd;
}
