#pragma once
#include "Line.h"
#include <vector>
#include <string>

class Player;
class RayEngine;
class Projectile;

class Level
{
	friend Player;
	friend RayEngine;
	friend Projectile;
public:
	Level(std::string);
	Level();
	~Level();
	void loadColouredLines(char*);
	void loadColouredTexturedLines(char*);
	std::string getName();
private:
	std::string name;
	std::vector<Line*> lines;
	bool textured = false;
};