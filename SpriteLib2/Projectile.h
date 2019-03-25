#pragma once
#include "spritelib\sprite.h"
#include "Circle.h"
#include "Level.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "cooldown.h"

typedef std::unordered_map<std::string, spritelib::Sprite> SpriteMap;
//share static data between player, rayengine and projectile
class Player;
class RayEngine;

enum OTE {	//over time effects
	None,
	Poison,
	Heal,
	DefChange,
	SpdChange
};

enum class Teams {
	Mages,
	Militians
};

class Projectile
{
	friend Player;
	friend RayEngine;
public:
	Projectile();
	Projectile(const Point&, const float&, const float&, const float&, const float&, std::string);
	//Projectile(const Point&, const float&, const float&, const float&, const float&, std::vector<std::string>&);
	static void changeLevel(const Level&);
    virtual bool move();
	static void loadSprites(const SpriteMap&);
    void draw();
	bool inFov(const Point&, const Point&, Point&);
	bool collideCircle(Circle&);
protected:
	bool friendly;	//decide this value in constructor
	//bool selfCast //also determined in the constructor
	//returns the index of the corresponding effectDuration
	virtual bool projectileEffect(float&, float&, float&, float&, float&, unsigned int&);
	static SpriteMap sprites;
	std::string image = "";
	std::string owner = "";
	std::vector<std::string> animationSet;
    static unsigned int numLines;
    static Level level;
    float _cosRot = 0.0f, _sinRot = 0.0f, _speed = 0.0f, _damage = 0.0f;
    bool collideLine(Circle&); //Takes a new position as an argument and tests it against the lines of the level
	Circle body;
	Teams team;
};