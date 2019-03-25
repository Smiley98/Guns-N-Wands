#include "Striker.h"

Striker::Striker()
{
}

Striker::~Striker()
{
}

// projectiles
Projectile* Striker::shootBasic(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0],imgArray[0]);
}
Projectile* Striker::shootSpecial_1(Point pos, float rot) {
	return new Explosion(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]); // steam cloud
}
Projectile* Striker::shootSpecial_2(Point pos, float rot) {
	static int fireRotation = rot;
	return new Projectile(pos, rot + (fireRotation += 10), projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]); // 360 degree ice spikes shot
}
Projectile* Striker::shootSpecial_3(Point pos, float rot) {
	return new Thrown(pos, rot, projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]); // fire grenade
}
Projectile* Striker::shootSpecial_4(Point pos, float rot) {
	return new SlowShot(pos, rot, projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]); // make people slow
}

// getters
cooldown& Striker::getCool(const int& index)
{
	return this->cooldowns[index];
}
cooldown& Striker::getDelay(const int& index)
{
	return this->delays[index];
}
cooldown& Striker::getGlobalCool()
{
	return this->globalCooldown;
}
float& Striker::getDefence() {
	return this->defence;
}
float& Striker::getDef() {
	return this->def;
}
float& Striker::getSpeed() {
	return this->moveSpeed;
}
float& Striker::getSpd() {
	return this->spd;
}