#pragma once
#include "spritelib\spritelib.h"
#include <functional>

class Button {
public:
	Button(int, int, int, int, const std::function<void()>&);
	Button(int, int, int, const std::function<void()>&);
	~Button();
	void draw();
	void onClick();
	void setPicture(spritelib::Sprite);
	bool clicked();
private:
	int x_, y_, w_, h_;
	bool mouseOver();
	std::function<void()> behaviour_;
	static sf::VideoMode resolution;
	void centreHorizontal();
	spritelib::Sprite picture;
};