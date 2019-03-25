#include "Destroyer.h"

Destroyer::Destroyer()
{
}

Destroyer::~Destroyer()
{
}

Projectile* Destroyer::shootBasic(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0], imgArray[0]);
}

Projectile* Destroyer::shootSpecial_1(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]);
}

Projectile* Destroyer::shootSpecial_2(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]);
}

Projectile* Destroyer::shootSpecial_3(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]);
}

Projectile* Destroyer::shootSpecial_4(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]);
}

cooldown& Destroyer::getCool(const int& index)
{
	return this->cooldowns[index];
}

cooldown& Destroyer::getDelay(const int& index)
{
	return this->delays[index];
}

cooldown& Destroyer::getGlobalCool()
{
	return this->globalCooldown;
}

float& Destroyer::getDefence() {
	return this->defence;
}
float& Destroyer::getDef() {
	return this->def;
}
float& Destroyer::getSpeed() {
	return this->moveSpeed;
}
float& Destroyer::getSpd() {
	return this->spd;
}

float& Destroyer::projectileRadius(const int& index)
{
	return this->projectileRadii[index];
}
