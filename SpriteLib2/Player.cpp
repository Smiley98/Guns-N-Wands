#include "Player.h"
#include "KeyEvents.h"
#include <cmath>
#include <iostream>

std::vector<Projectile*> Player::projectiles;
decInfo Player::decorationData;
float Player::viewDistance;
float Player::halfHeight;
unsigned int Player::numPlayers;
#define PI (3.1415)

Player::Player(Teams t, Point position, float rotation, Point screenCentre, spritelib::Sprite *sky, Character* character, std::vector<cooldown> cool, bool xin) :
	_position(position), _rot(rotation), _screenCentre(sf::Vector2i(screenCentre._x, screenCentre._y)), _character(character), aniCools(cool), team(t) {
	body._rad = _character->radius;		//base character radius
	_moveSpeed = _character->getSpeed();//derived character speed
	_rotSpeed = 6.00f * 3.14f / 180.0f;
	skybox = sky;
	_dash.changeLength(cooldown::toMilliseconds(500));
	_dashCD.changeLength(cooldown::toMilliseconds(2000));
	spawnPoint = position;
	_character->getGlobalCool().changeLength(0);
	numPlayers++;
	name += std::to_string(numPlayers);

	if (xin) {
		processInput = &Player::xin;
	}
	else {
		processInput = &Player::keyboard;
	}

	for (int i = 0; i < Character::numEffects; i++)
		_character->effectDurations[i].expire();
}

Player::~Player()
{
	for (int i = 0; i < projectiles.size(); i++) {
		delete projectiles[i];
		projectiles.erase(projectiles.begin() + i);
	}
}

void Player::updateMap(const Level& level) {
	Projectile::changeLevel(level);
}

void Player::setViewDist(const sf::VideoMode& resolution)
{
	viewDistance = resolution.width / 2.0f;	//distance from player to plane (camera to monitor)
	halfHeight = resolution.height / 2.0f;
}

void Player::move() {
	(this->*(processInput))();	//dereference the member function, then call it
	//update movement values based on input
	skybox->HorizontalScroll(_scrollSpeed * -50);
	if (_rot < 0)
		_rot += 6.28f;
	_rot = std::fmod(_rot, 6.28f);	//clamp rotation between -360 and 360 degrees (-2Pi to 2Pi).
	_cosRot = std::cosf(_rot);
	_sinRot = std::sinf(_rot);
	_direction = Point(_cosRot, _sinRot);
	
	//create segment from current position to desired position, if no intersection, update position. if intersection, return
	Point newPosition{ _position._x + _xStep, _position._y + _yStep };
	Line movSeg(_position, newPosition);
	if (isColliding(movSeg)) return;
	_position = newPosition;
	body._centre = newPosition;
}

void Player::loadSprites(const SpriteMap& map, const decInfo& decorationInfo)
{
	Projectile::loadSprites(map);
	decorationData = decorationInfo;
}

bool Player::isColliding(Line playerMoveSegment) {	//do stuff with surface normal in future to slide along walls!
	for (int i = 0; i < Projectile::numLines; i++) {
		if (playerMoveSegment.intersect(*Projectile::level.lines[i])) return true;
	}
	return false;
}

float Player::getX()
{
	return _position._x;
}

float Player::getY()
{
	return _position._y;
}

void Player::shoot()
{
	cooldown& spellCool = _character->getCool(_selectedSpell);
	cooldown& globalCool = _character->getGlobalCool();
	if (spellCool.ready() && (globalCool.ready() || _selectedSpell == 0 || _selectedSpell == _previousSpell)) {
		spellCool.reset();
		globalCool = _character->getDelay(_selectedSpell);
		globalCool.reset();

		float radius = _character->projectileRadius(_selectedSpell) + body._rad * 0.95f;	//*0.95 so projectile ALWAYS collides with player.
		Point p(_position._x + _cosRot * radius, _position._y + _sinRot * radius);

		_previousSpell = _selectedSpell;	//this assignment can happen any time in this function

		if(_selectedSpell == 0)
			projectiles.push_back(_character->shootBasic(p, _rot));
		if(_selectedSpell == 1)
			projectiles.push_back(_character->shootSpecial_1(p, _rot));
		else if (_selectedSpell == 2)
			projectiles.push_back(_character->shootSpecial_2(p, _rot));
		else if (_selectedSpell == 3)
			projectiles.push_back(_character->shootSpecial_3(p, _rot));
		else if (_selectedSpell == 4)
			projectiles.push_back(_character->shootSpecial_4(p, _rot));

		projectiles[projectiles.size() - 1]->team = team;
		projectiles[projectiles.size() - 1]->owner = name;
	}
}

void Player::xin()
{
	isMoving = false;
	//poll controller
	controller.DownloadPackets();
	controller.GetSticks(0, lStick, rStick);

	_scrollSpeed = rStick.xAxis * _rotSpeed;
	_rot -= _scrollSpeed;

	float controllerAngle = std::atan2(-lStick.xAxis, lStick.yAxis);
	float cosStep = std::cos(controllerAngle + _rot);
	float sinStep = std::sin(controllerAngle + _rot);
	bool mult = std::abs(lStick.xAxis) > 0.25f || std::abs(lStick.yAxis) > 0.25f ? true : false;

	_xStep = cosStep * _moveSpeed * mult;
	_yStep = sinStep * _moveSpeed * mult;

	if (mult)
		isMoving = true;

	if(controller.GetButton(0, XBox::RB)) {
		_selectedSpell = Spells::basic;
		shoot();
	}
	else if (controller.GetButton(0, XBox::X)) {
		_selectedSpell = Spells::one;
		shoot();
	}
	else if (controller.GetButton(0, XBox::Y)) {
		_selectedSpell = Spells::two;
		shoot();
	}
	else if (controller.GetButton(0, XBox::B)) {
		_selectedSpell = Spells::three;
		shoot();
	}
	else if (controller.GetButton(0, XBox::A)) {
		_selectedSpell = Spells::four;
		shoot();
	}
}

void Player::keyboard()
{
	//reset the multiplicitives every call, then check if the key is up
	_xStep = 0.0f;
	_yStep = 0.0f;
	isMoving = false;

	_scrollSpeed = ((viewDistance - sf::Mouse::getPosition().x) / viewDistance) * _rotSpeed * 5.0f;	//define scroll speed based on how far away from centre x
	_rot += _scrollSpeed;
	sf::Mouse::setPosition(_screenCentre);
	float rotRight = _rot - 1.57f;
	float rotLeft = _rot + 1.57f;
	float rotBack = _rot + 3.14f;

	//Keydown:
	if (KeyEvents::keysDown[KeyEvents::A]) {
		_xStep += _moveSpeed * std::cos(rotLeft);
		_yStep += _moveSpeed * std::sin(rotLeft);
		isMoving = true;
	}
	else if (KeyEvents::keysDown[KeyEvents::D]) {
		_xStep += _moveSpeed * std::cos(rotRight);
		_yStep += _moveSpeed * std::sin(rotRight);
		isMoving = true;
	}
	if (KeyEvents::keysDown[KeyEvents::W]) {
		_xStep += _moveSpeed * std::cos(_rot);
		_yStep += _moveSpeed * std::sin(_rot);
		isMoving = true;
	}
	else if (KeyEvents::keysDown[KeyEvents::S]) {
		_xStep += _moveSpeed * std::cos(rotBack);
		_yStep += _moveSpeed * std::sin(rotBack);
		isMoving = true;
	}
	
	//projectiles
	if (KeyEvents::keysDown[KeyEvents::Num1]) {
		_selectedSpell = Spells::one;
		shoot();
	}
	else if (KeyEvents::keysDown[KeyEvents::Num2]) {
		_selectedSpell = Spells::two;
		shoot();
	}
	else if (KeyEvents::keysDown[KeyEvents::Num3]) {
		_selectedSpell = Spells::three;
		shoot();
	}
	else if (KeyEvents::keysDown[KeyEvents::Num4]) {
		_selectedSpell = Spells::four;
		shoot();
	}
	else if (KeyEvents::keysDown[KeyEvents::Space]) {
		_selectedSpell = Spells::basic;
		shoot();
	}
}

void Player::setCharacter(Character* character)
{
	_character = character;

	_character->getGlobalCool().changeLength(0);

	for (int i = 0; i < Character::numEffects; i++)
		_character->effectDurations[i].expire();
}

Point & Player::getPos()
{
	return _position;
}

float& Player::getRot()
{
	return _rot;
}

spritelib::Sprite* Player::getSkyBox()
{
	return skybox;
}

void Player::update(Player* otherPlayer)
{
	//move based on input, then handle projectiles
	skybox->draw();
	move();
	//for all projectiles:
	for (unsigned int i = 0; i < projectiles.size(); i++) {
		bool applyEffect = false;
		if (projectiles[i]->collideCircle(body)) {	//if there is a collision between a player and a projectile:
			if (projectiles[i]->team == team && projectiles[i]->friendly) {
//if ((projectiles[i]->selfCast && projectiles[i]->owner == name) || (!projectiles[i]->selfCast && projectiles[i]->owner != name))	//there is no more self-cast cause no teamates
					applyEffect = true;	//heal if self || ally is hit and spell is friendly && allowed to heal self || allowed to heal ally (stops supports from healing themselves when trying to heal an ally)
			}
			else if (projectiles[i]->team != team && !projectiles[i]->friendly) {
				applyEffect = true;		//damage if projectile isn't friendly and hits an enemy
			}
		}

		if(applyEffect){
			unsigned int spellEffectIndex = OTE::None;	//initial effect is nothing. Defined based on the projectile
			bool outcome = projectiles[i]->projectileEffect(_character->health, _character->getDefence(), _character->getDef(), _character->getSpeed(), _character->getSpd(), spellEffectIndex);
			_character->effectDurations[spellEffectIndex].reset();	//applies over time effect if applicable
			//primary case in which projectiles may be deleted (if a player is hit), return value of projectileEffect decides if the projectile persists.
			if (outcome) {
				delete projectiles[i];
				projectiles.erase(projectiles.begin() + i);
			}
			
			//handle death
			if (_character->health <= 0) {
				spawnPoint = getRespawn(otherPlayer);
				_position = spawnPoint;
				_character->health = 100.0f;
				lives--;
			}
		}

		//secondary case in which projectiles may be deleted (they expire or hit a wall)
		else if (!projectiles[i]->move()) {
			delete projectiles[i];
			projectiles.erase(projectiles.begin() + i);
		}

		//tertiary case: a projectile has tunnelled through the level and is way outside the map
		//could optimise this by storing the greatest and smallest x's and y's of the map and then comparing.
		else if (std::abs(projectiles[i]->body._centre._x) > 2000.0f || std::abs(projectiles[i]->body._centre._y) > 2000.0f) {
			delete projectiles[i];
			projectiles.erase(projectiles.begin() + i);
		}
	}
//End iterating through projectiles

	//Apply effect if affected (grammar? xD)
	for (int i = 0; i < Character::numEffects; i++) {	//for every POSSIBLE effect
		//if an effect is on cooldown, that means its applied
		//CHECK IF THE PLAYER IS AFFECTED BY THE iTH EFFECT.
		if (!_character->effectDurations[i].ready()) {
			//apply the effect based off of which index is on cooldown
			if (i == OTE::Poison) {
				_character->health -= 0.01f;
			}
			if (i == OTE::Heal) {
				_character->health += 0.01f;
			}
		}
		else {	//otherwise, reset states:
			if (i == OTE::DefChange) {
				_character->defence = _character->def;
			}
			if (i == OTE::SpdChange) {
				_character->moveSpeed = _character->spd;
			}
		}
	}
}

void Player::drawAssist(Point targetPos, std::string image, Player* otherPlayer, float a_scale) {
	//b to a = a - b, thus to target = target - position
	float dx = targetPos._x - _position._x;	//horizontal distance from target to player
	float dy = targetPos._y - _position._y;	//vertical distance from target to player
	Point toTarget(dx, dy);
	if (toTarget*_direction > 0.7071f) {}	//operator* is actually dot product, smh... Anyways, if the angle between the player and object is less than
	else return;							//fov, continue
	float smallestDistance = dx*dx + dy*dy;	//distance from player to sprite
	Line lineToTarget(_position, targetPos);
	for (int i = 0; i < Projectile::numLines; i++) {	//make sure nothing is behind the object
		Point poi;
		if (lineToTarget.intersect(*Projectile::level.lines[i], poi)) {
			float mag = std::pow(_position._x - poi._x, 2) + std::pow(_position._y - poi._y, 2);
			if (mag < smallestDistance) return;
		}
	}
	//Let the fun begin:
	float dist = std::sqrt(dy*dy + dx*dx);				//gotta sqrt now because we're doing more than just comparing
	float spriteAngle = std::atan2f(dy, dx) - _rot;		//the angle between player 
	float scale = std::abs(viewDistance / (std::cosf(spriteAngle) * dist));	//cosine to correct fisheye. Viewdistance is basically a static z coordinate.
	float x = std::tanf(spriteAngle) * viewDistance;	//make a triangle -- go viewdistance units out of monitor, then spawn sprite at angle from camera

	//sf::Vector2u imgSize = spritelib::Sprite::m_images[image].getSize();	//assign in character for projectile radii
	//std::cout << imgSize.x << "   " << imgSize.y << std::endl;
	Projectile::sprites[image].set_position(viewDistance - x, halfHeight);
	//0.5625 is 1080 divided by 1920.
	//50 (default value of a_scale) is an arbitrary number that makes things look decent
	Projectile::sprites[image].set_scale(scale*a_scale * 0.5625f, scale*a_scale);
	Projectile::sprites[image].draw();
	
	animationAssist(image, otherPlayer);
}

void Player::animationAssist(std::string image, Player * otherPlayer)
{
	float otherRotation = _rot - otherPlayer->_rot;

	if (otherRotation < 0)
		otherRotation = (2 * PI) + otherRotation;

	if ((otherRotation >= 7.0f * PI / 4.0f) || (otherRotation < PI / 4.0f))
	{
		if (aniCools[0].ready() && otherPlayer->isMoving) {
			Projectile::sprites[image].set_sprite_frame_size(32, 70).set_animation("run_b");
			Projectile::sprites[image].next_frame();
			aniCools[0].reset();
		}

		else if (!otherPlayer->isMoving)
			Projectile::sprites[image].set_sprite_frame_size(32, 70).set_animation("idle_b");
	}

	else if (otherRotation <= 3.0f * PI / 4.0f)
	{
		if (aniCools[0].ready() && otherPlayer->isMoving) {
			Projectile::sprites[image].set_sprite_frame_size(45, 50).set_animation("run_r");
			Projectile::sprites[image].next_frame();
			aniCools[0].reset();
		}

		else if (!otherPlayer->isMoving)
			Projectile::sprites[image].set_sprite_frame_size(45, 50).set_animation("idle_r");
	}

	else if (otherRotation <= 5.0f * PI / 4.0f)
	{
		if (aniCools[0].ready() && otherPlayer->isMoving) {
			Projectile::sprites[image].set_sprite_frame_size(32, 70).set_animation("run_f");
			Projectile::sprites[image].next_frame();
			aniCools[0].reset();
		}

		else if (!otherPlayer->isMoving)
			Projectile::sprites[image].set_sprite_frame_size(32, 70).set_animation("idle_f");
	}

	else
	{
		if (aniCools[0].ready() && otherPlayer->isMoving) {
			Projectile::sprites[image].set_sprite_frame_size(45, 50).set_animation("run_l");
			Projectile::sprites[image].next_frame();
			aniCools[0].reset();
		}

		else if (!otherPlayer->isMoving)
			Projectile::sprites[image].set_sprite_frame_size(45, 50).set_animation("idle_l");
	}
}

void Player::render3D(Player* otherPlayer)
{
	for (auto itr = decorationData.begin(); itr != decorationData.end(); itr++)
		drawAssist(itr->second->_position, itr->first, itr->second);	//itr->second is a player pointer

	for (int i = 0; i < projectiles.size(); i++)	//we pass in a player because we want to render something at a given position relative to the viewer
		drawAssist(projectiles[i]->body._centre, projectiles[i]->image, this, projectiles[i]->body._rad*2.0f);
}

void Player::render2D(Player& p1, Player& p2)
{
	for (int i = 0; i < projectiles.size(); i++)
		projectiles[i]->body.draw();

	p1.body.draw();
	p2.body.draw();
	spritelib::Shapes::draw_line(p1._position._x, p1._position._y, p1._position._x + p1._cosRot * 20.0f, p1._position._y + p1._sinRot * 20.0f);
	spritelib::Shapes::draw_line(p2._position._x, p2._position._y, p2._position._x + p2._cosRot * 20.0f, p2._position._y + p2._sinRot * 20.0f);
}

Point Player::getRespawn(Player * otherPlayer)
{
	float distance, longestDistance = 0;
	Point spawn;

	for (int i = 0; i < spawns.size(); i++)
	{
		distance = sqrt(pow(spawns[i]._x - otherPlayer->getX(), 2) + pow(spawns[i]._y - otherPlayer->getY(), 2));

		if (distance > longestDistance)
		{
			longestDistance = distance;
			spawn = spawns[i];
		}
	}

	return spawn;
}

void Player::lerp(Point startPoint, Point endPoint, float endRot, float totalTime, float dt)
{
	float xDistance = endPoint._x - startPoint._x;
	float yDistance = endPoint._y - startPoint._y;

	shift(startPoint._x + xDistance * (dt / totalTime), startPoint._y + yDistance * (dt / totalTime));

	float rotDif = endRot - _rot;

	_rot += (rotDif * (dt / (totalTime * 10)));

	skybox->HorizontalScroll(rotDif * (dt * -5));
}

void Player::operator=(Player& p)
{
	*this = p;
}

void Player::shift(float x, float y)
{
	_position._x = x;
	body._centre._x = x;
	_position._y = y;
	body._centre._y = y;
}