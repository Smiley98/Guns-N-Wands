#include "Sniper.h"

Sniper::Sniper()
{
	srand((unsigned)time(0));
}

Sniper::~Sniper()
{
}

Projectile* Sniper::shootBasic(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[0], projectileSpeeds[0], spellDamages[0], imgArray[0]);
}
Projectile* Sniper::shootSpecial_1(Point pos, float rot) {
	return new SlowShot(pos, rot, projectileRadii[1], projectileSpeeds[1], spellDamages[1], imgArray[1]); // make enemies slow for easier sniping
}
Projectile* Sniper::shootSpecial_2(Point pos, float rot) {
	return new Thrown(pos, rot, projectileRadii[2], projectileSpeeds[2], spellDamages[2], imgArray[2]); // grenade (may change to be more C4 sachel-charge-like
}
Projectile* Sniper::shootSpecial_3(Point pos, float rot) {
	return new Projectile(pos, rot + (rand() % (11) - 5), projectileRadii[3], projectileSpeeds[3], spellDamages[3], imgArray[3]); // mini-shotgun-pistol
}
Projectile* Sniper::shootSpecial_4(Point pos, float rot) {
	return new Projectile(pos, rot, projectileRadii[4], projectileSpeeds[4], spellDamages[4], imgArray[4]); // BOOM! HEADSHOT! (extra damaging bullet)
}

// getters
cooldown& Sniper::getCool(const int& index)
{
	return this->cooldowns[index];
}
cooldown& Sniper::getDelay(const int& index)
{
	return this->delays[index];
}
cooldown& Sniper::getGlobalCool()
{
	return this->globalCooldown;
}
float& Sniper::getDefence() {
	return this->defence;
}
float& Sniper::getDef() {
	return this->def;
}
float& Sniper::getSpeed() {
	return this->moveSpeed;
}
float& Sniper::getSpd() {
	return this->spd;
}
