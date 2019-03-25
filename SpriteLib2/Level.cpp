#include "Level.h"
#include <cstdio>
#pragma warning (disable : 4996)
Level::Level(std::string n) : name(n)
{
}
Level::Level()
{
}

Level::~Level()
{
	/*for (int i = 0; i < lines.size(); i++) {
		delete lines[i];
	}*/
}

void Level::loadColouredLines(char* filePath)
{
	textured = false;
	std::FILE* fp = std::fopen(filePath, "r");
	if (fp == nullptr) {
		perror(filePath);
		std::getchar();
		std::printf("\nFile not found. P");
	}
	float x1 = 0.0f;
	float y1 = 0.0f;
	float x2 = 0.0f;
	float y2 = 0.0f;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float h = 0.0f;
	int counter = 0;
	while (std::fscanf(fp, "%f%f%f%f%f%f%f%f", &x1, &y1, &x2, &y2, &r, &g, &b, &h) != EOF) {
		//std::printf("Line %i: %f %f %f %f %f %f %f %f\n", counter, x1, y1, x2, y2, r, g, b, h);
		lines.push_back(new Line(Point{ x1, y1 }, Point{ x2,y2 }));
		lines[counter]->c.x = r;
		lines[counter]->c.y = g;
		lines[counter]->c.z = b;
		lines[counter]->h = h;
		lines[counter]->w = lines[counter]->calcLength();
		lines[counter]->number_ = counter;
		counter++;
	}
	std::fclose(fp);
}

void Level::loadColouredTexturedLines(char* filePath)
{
	textured = true;
	std::FILE* fp = std::fopen(filePath, "r");
	if (fp == nullptr) {
		perror(filePath);
		std::getchar();
		std::printf("\nFile not found. P");
	}
	float x1 = 0.0f;
	float y1 = 0.0f;
	float x2 = 0.0f;
	float y2 = 0.0f;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float h = 0.0f;
	char id[16];
	int counter = 0;
	while (std::fscanf(fp, "%f%f%f%f%f%f%f%f%s", &x1, &y1, &x2, &y2, &r, &g, &b, &h, id) != EOF) {
		//std::printf("Line %i: %f %f %f %f %f %f %f %f %s\n", counter, x1, y1, x2, y2, r, g, b, h, id);
		lines.push_back(new Line(Point{ x1, y1 }, Point{ x2,y2 }));
		lines[counter]->c.x = r;
		lines[counter]->c.y = g;
		lines[counter]->c.z = b;
		lines[counter]->h = h;
		lines[counter]->w = lines[counter]->calcLength();
		lines[counter]->number_ = counter;
		memcpy(lines[counter]->texID, id, sizeof id);
		//std::printf("Texture: %s\n", lines[counter]->texID);
		counter++;
	}
	std::fclose(fp);
}

std::string Level::getName()
{
	return name;
}
