#include "Mender.h"

Mender::Mender()
{
	srand((unsigned)time(0));
}


Mender::~Mender()
{
}

// projectiles
Projectile* Mender::shootBasic(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0], imgArray[0]);
}
Projectile* Mender::shootSpecial_1(Point pos, float rot) {
	return new HealingShot(pos, rot, projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]); // heal others
}
Projectile* Mender::shootSpecial_2(Point pos, float rot) {
	return new DefBoost(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]); // defence boost for self and broad area
}
Projectile* Mender::shootSpecial_3(Point pos, float rot) {
	return new Thrown(pos, rot, projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]); // grenade
}
Projectile* Mender::shootSpecial_4(Point pos, float rot) {
	return new PoisonGas(pos, rot + (rand() % (11) - 5), projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]); // poison gas
}

// getters
cooldown& Mender::getCool(const int& index)
{
	return this->cooldowns[index];
}
cooldown& Mender::getDelay(const int& index)
{
	return this->delays[index];
}
cooldown& Mender::getGlobalCool()
{
	return this->globalCooldown;
}
float& Mender::getDefence() {
	return this->defence;
}
float& Mender::getDef() {
	return this->def;
}
float& Mender::getSpeed() {
	return this->moveSpeed;
}
float& Mender::getSpd() {
	return this->spd;
}