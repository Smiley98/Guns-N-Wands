#pragma once
#include "spritelib\spritelib.h"
#include "controller.h"
#include "Circle.h"
#include "Projectile.h"
#include "Character.h"
#include <vector>
typedef std::unordered_map<std::string, Player*> decInfo;

enum Spells {
	basic,
	one,
	two,
	three,
	four,
};

class RayEngine;

class Player
{
	friend RayEngine;
public:
	Player(Teams, Point, float, Point, spritelib::Sprite*, Character*, std::vector<cooldown>, bool);
	~Player();
	void move();
	static void loadSprites(const SpriteMap&, const decInfo&);
	static void updateMap(const Level&);
	static void setViewDist(const sf::VideoMode&);
	bool isColliding(Line);
	float getX();
	float getY();
	float getHealth() {
		return _character->health;
	}
	int getLives()
	{
		return lives;
	}
	void setCharacter(Character*);
	Point& getPos();
	float& getRot();
	spritelib::Sprite* getSkyBox();
	void update(Player* otherPlayer);
	void render3D(Player* otherPlayer);
	static void render2D(Player&, Player&);
	Point getRespawn(Player* otherPlayer);
	void lerp(Point, Point, float, float, float);
	
	void operator=(Player&);

	void shift(float, float);
private:
	int _selectedSpell = 0;
	int _previousSpell = 0;

	float _rot;			//rotation in radians
	float _cosRot;		//x component of rotation unit vector
	float _sinRot;		//y component of rotation unit vector

	float _moveSpeed;	//forward-backward velocity
	float _rotSpeed;	//rate of rotation
	float _scrollSpeed;

	float _xStep;
	float _yStep;
	Point _position;
	Point _direction;

	Circle body;
	Teams team;
	Character* _character;

	const sf::Vector2i _screenCentre;
	sf::Vector2i _mousePos;

	spritelib::Sprite* skybox;
	std::vector<cooldown> aniCools;
	Point spawnPoint;
	std::vector<Point> spawns{
		Point(525, 25),
		Point(1975, 25),
		Point(25, 1975),
		Point(1575, 1975)
	};
	cooldown _dash;
	cooldown _dashCD;
	bool _isDashing;

	int lives = 5;

	//projectiles
	static std::vector<Projectile*> projectiles;
	static float viewDistance;
	static float halfHeight;
	static decInfo decorationData;	//map of strings and points
	void shoot();
	std::string name = "p";
	static unsigned int numPlayers;
	
	//input members
	XBox::XBoxInput controller;
	XBox::Stick lStick;
	XBox::Stick rStick;
	void xin();
	void keyboard();
	void (Player::*processInput)();
	bool isMoving = false;

	//sub-renderers
	void drawAssist(Point, std::string, Player*, float = 50.0f);
	void animationAssist(std::string, Player*);
};