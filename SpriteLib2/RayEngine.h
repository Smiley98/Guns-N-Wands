#pragma once
#include "Player.h"
#include "spritelib\spritelib.h"
#include "spritelib\shader.h"
#include <cmath>
typedef spritelib::ShaderProgram* sptr;
typedef spritelib::math::Matrix4x4* mat4ptr;
#define DRAW2D FALSE;
class RayEngine
{
public:
	RayEngine(sptr, mat4ptr, float = 0.0f);
	~RayEngine();
	void castRays(const Player&/*, const Point&, const float& = 0.0f*/);
	void drawMiniMap();
	static void setResolution(const sf::VideoMode&);
	static void setMap(const Level&);
	static std::string getMapName();
private:
	float projectedWallHeight;
	float lineWidth;
	float step;

	//shader variables
	sptr sp;
	mat4ptr defaultShaderMatrix;

	//statics
	static sf::VideoMode resolution;
	static Point screenCentre;
	static float distPlayerToPlane;		//equal to screen width / tan(fov/2) ->fov is 90 and tan(45) is one so we don't need this formula cause the fov never changes. Better to not use decimals in calculations

	//inlines:
	void setTexture(std::string);
};

