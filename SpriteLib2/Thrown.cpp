#include "Thrown.h"

Thrown::Thrown(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName)
{
	detonator.changeLength(1000000000);
	detonator.reset();
	explosion_timer.changeLength(1000000000);
	fuse = 5;
	exp_phase = false;
}

Thrown::~Thrown()
{
}

bool Thrown::move() {
	if (detonator.ready()) // grenade phase
		exp_phase = true;

	if (!exp_phase) {
		fuse *= 2;
		_speed -= (_speed * (fuse / 100)); // decrease speed over time
		Circle updatedBody = Circle(Point(body._centre._x + _cosRot * _speed, body._centre._y + _sinRot * _speed), body._rad);
		if (!collideLine(updatedBody)) // if doesn't hit wall continue on, otherwise just stop because bouncin off of wall is out of scope at this point
			body = updatedBody;
		return true;
	}
	else { // explosion phase
		if (explosion_timer.ready())
			return false;

		// change sprite
		Circle updatedBody = Circle(Point(body._centre._x, body._centre._y), body._rad + 10.0f);
	}	
}

bool Thrown::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int& effectInd) {
	if (!exp_phase)
		exp_phase = true;
	else
		health -= (_damage -= _damage * (defence / 100.0f));
		return false; // do not destroy on contact with player
}