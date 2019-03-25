#include "RayEngine.h"
#include "spritelib\Sprite.h"
#include <iostream>
typedef spritelib::math::Vector3 vec3;

sf::VideoMode RayEngine::resolution;
float RayEngine::distPlayerToPlane;
Point RayEngine::screenCentre;

RayEngine::RayEngine(sptr s_sp, mat4ptr dsm, float numRays) :
	sp(s_sp), defaultShaderMatrix(dsm) {
	if (numRays == 0)
		numRays = resolution.width;
	lineWidth = resolution.width / numRays;
	step = 1.0f / (numRays * 0.5f);	//0.25 for vertical split, 0.5 for fullscreen. We're multiplying by 0.5 because we're going from -step to step so our range is effectively 2(step), thus we have to correct.
	spritelib::Shapes::set_line_width(lineWidth);
}

RayEngine::~RayEngine()
{
	sp->cleanup();
}

void RayEngine::castRays(const Player& player) {
	float x = std::cos(player._rot);
	float y = std::sin(player._rot);
	float line_x = 0.0f;

	sp->use_program();
	sp->set_int("mainTexture", 0);
	sp->set_mat4("projection", &defaultShaderMatrix->m_data[0]);

	const float fov = 60.0f;
	const float x_res = 1920.0f;
	//How many degrees to step per-pixel.
	const float slit = fov / x_res;
	//Start at far left, end up at far right.
	float ray_angle = fov / 2.0f - player._rot;


	for (float i = -1.0f; i < 1.0f; i += step) {
		line_x+=lineWidth;

		vec3 forward = vec3(x, y, 0.0f).normalize();
		vec3 right = forward.cross(vec3(0.0f, 0.0f, 1.0f));

		Point p1(player._position._x, player._position._y);
		//I think we actually have an fov of 180 degrees, or maybe its 90... I gotta touch up on the whole "tan half fov" thing / look into raycasting from a non-javascript aids tutorial.
		vec3 rayDir = vec3(forward.add(right.multiply(i))).normalize();
		Point p2(p1._x + rayDir.x * 10000.0f, p1._y + rayDir.y * 10000.0f);

		Line ray(p1, p2);
		Line closestLine;
		Point hitPoint;
		float smallestDistance = 999999999.0f;
		
		for (int j = 0; j < Projectile::numLines; j++) {
			if (ray.intersect(*Projectile::level.lines[j], p2)) {										//end the line at p2
				float mag = std::pow(p2._x - p1._x, 2.0f) + std::pow(p2._y - p1._y, 2.0f);	//distance of a line: delta x^2 + delta y^2, manhattan distance - no need to sqrt yet (thanks Guzy!)
				if (mag < smallestDistance) {												//reassign the smallest distance if the current ray is smaller and store the contents of that line
					smallestDistance = mag;													
					closestLine = *Projectile::level.lines[j];
					hitPoint = p2;
				}
			}
		}
#if DRAW2D
		ray._end = hitPoint;
		ray.c = closestLine.c;
		//ray.drawColour();
#else
		//cos(x) = u.v/||u||*||v||
		smallestDistance = std::sqrt(smallestDistance);
		float cosAngleBetweenPlayerAndRay = forward.dot(rayDir)/(forward.magnitude() * rayDir.magnitude());
		smallestDistance *= cosAngleBetweenPlayerAndRay;								//correct fisheye effect
		projectedWallHeight = (closestLine.h / smallestDistance)*distPlayerToPlane;
		if (Projectile::level.textured) {
			float dx = hitPoint._x - closestLine._start._x;
			float dy = hitPoint._y - closestLine._start._y;
			float dist = std::sqrt(dx*dx + dy*dy);
			//This is a magic number. It influences horizontal texture sampling. Always gonna be magic cause textures and distances are different. 
			float U = (dist / 50.0f);
			setTexture(closestLine.texID);
			float col[3] = { closestLine.c.x, closestLine.c.y, closestLine.c.z };
			sp->set_vec3("color", &col[0]);
			float pos[3] = { line_x, screenCentre._y, 0.0f };
			sp->set_vec3("posA", &pos[0]);
			sp->set_float("wallHeight", projectedWallHeight);
			sp->set_float("U", U);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		else {
			Line line3(Point{ line_x, screenCentre._y - projectedWallHeight / 2.0f }, Point{ line_x, screenCentre._y + projectedWallHeight / 2.0f });
			line3.c = closestLine.c;
			line3.drawColour();
		}
		
#endif
	}
#if DRAW2D
	drawMiniMap();
#endif
}

void RayEngine::drawMiniMap()
{
	for (int i = 0; i < Projectile::numLines; i++) {
		Projectile::level.lines[i]->drawColour();
	}
}

void RayEngine::setResolution(const sf::VideoMode& res)
{
	resolution = res;
	distPlayerToPlane = resolution.width / 2.0f;
	screenCentre = Point(resolution.width / 2.0f, resolution.height / 2.0f);
	Player::setViewDist(res);
}

void RayEngine::setMap(const Level& map)
{
	Player::updateMap(map);
}

std::string RayEngine::getMapName()
{
	return Projectile::level.name;
}

inline void RayEngine::setTexture(std::string id)
{
	glBindTexture(GL_TEXTURE_2D, spritelib::Sprite::m_images[id].getNativeHandle());
}
