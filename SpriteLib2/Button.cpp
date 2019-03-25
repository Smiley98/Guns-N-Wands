#include "Button.h"
#include <iostream>

sf::VideoMode Button::resolution = sf::VideoMode::getDesktopMode();
Button::Button(int x, int y, int w, int h, const std::function<void()>& behaviour) : x_(x), y_(y), w_(w), h_(h), behaviour_(behaviour)
{
}

Button::Button(int y, int w, int h, const std::function<void()>& behaviour) : y_(y), w_(w), h_(h), behaviour_(behaviour)
{
	centreHorizontal();
}

Button::~Button()
{
}

bool Button::mouseOver()
{
	sf::Vector2i mouse = sf::Mouse::getPosition();
	int y = resolution.height - mouse.y;
	if (mouse.x < x_) return false;
	if (mouse.x > x_ + w_) return false;
	if (y < y_) return false;
	if (y > y_ + h_) return false;
	return true;
}

void Button::draw()
{
	spritelib::Shapes::set_color(1.0f, 1.0f, 1.0f);
	spritelib::Shapes::draw_rectangle(true, x_, y_, w_, h_);
	picture.draw();

}

void Button::onClick()
{
	if (mouseOver() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		behaviour_();
}

void Button::setPicture(spritelib::Sprite sprite)
{
	picture = sprite;
	picture.set_center(0.0f, 0.0f).set_scale(w_, h_).set_position(x_, y_);
}

bool Button::clicked()
{
	if (mouseOver() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		return true;
	return false;
}

void Button::centreHorizontal()
{
	x_ = resolution.width / 2 - w_ / 2;
}
