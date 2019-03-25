#include <iostream>
#include <cmath>
#include "Projectile.h"
#include "spritelib\shapes.h"
#include <iostream>
Level Projectile::level;
unsigned int Projectile::numLines;
SpriteMap Projectile::sprites;

Projectile::Projectile() {}

Projectile::Projectile(const Point &position, const float &rotation, const float &radius, const float &speed, const float &damage, std::string imageName) :
body(Circle(position, radius)),
_cosRot(std::cosf(rotation)),
_sinRot(std::sinf(rotation)),
_speed(speed), _damage(damage), image(imageName),
friendly(false) {}

void Projectile::changeLevel(const Level& map)
{
    level = map;
    numLines = level.lines.size();
}

bool Projectile::move() {
    //Line newLine = Line(Point(line._end._x, line._end._y), Point(line._end._x + _cosRot * _speed, line._end._y + _sinRot * _speed));
    Circle updatedBody = Circle(Point(body._centre._x  + _cosRot * _speed, body._centre._y + _sinRot * _speed), body._rad);
    if (collideLine(updatedBody))
        return false;
	body = updatedBody;
    return true;
}

void Projectile::loadSprites(const SpriteMap& map)
{
	sprites = map;
}

void Projectile::draw()
{
	if (image == "") {
		spritelib::Shapes::set_color(1, 0, 0);
		body.draw();
	}
	else {
		sprites[image].set_position(body._centre._x, body._centre._y);
		sprites[image].set_scale(body._rad, body._rad);
		sprites[image].draw();
	}
}

bool Projectile::projectileEffect(float& health, float& defence, float& def, float& speed, float& spd, unsigned int&) {
	health -= _damage;
	return true;	//delete case: if true, delete
}

bool Projectile::inFov(const Point& referencePos, const Point& referenceDir, Point& outVec)	//player's position
{
	//works from 90 to 180 and 270 to 360
	Point toPlayer = (body._centre._x - referencePos._x, body._centre._y - referencePos._y);
	//Point toPlayer = (referencePos._x - body._centre._x, referencePos._y - body._centre._y);
	toPlayer /= toPlayer.mag();			//normalize to get direction from projectile to player
	float dp = toPlayer * referenceDir;	//dot product with player direction
	std::cout << std::atan2(referenceDir._y, referenceDir._x)*180.0f/3.14f<<std::endl;
	if (dp > 0.707f) return true;		//0.707 = cos(45) = half fov
	return false;
}

inline bool Projectile::collideLine(Circle& circle)
{
    for (unsigned int i = 0; i < numLines; i++) {
        if (circle.intersect(*level.lines[i]))
            return true;
    }
    return false;
}

bool Projectile::collideCircle(Circle& circle)
{
	if (body.intersect(circle))
		return true;
	return false;
}