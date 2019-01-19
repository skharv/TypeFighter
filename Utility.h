#ifndef UTILITY_H
#define UTILITY_H

#include <Box2D\Box2D.h>
#include "SFML\Graphics.hpp"

namespace ut
{
	const char* const HITBOX = "HITBOX";
	const char* const ENEMY = "ENEMY";
	const char* const PLAYER = "PLAYER";
	const char* const KILL = "KILL";
	const char* const DIE = "DIE";
	const char* const INVULNERABLE = "INVULNERABLE";
	const char* const DESTRUCTABLE = "DESTRUCTABLE";
	const char* const WORLD = "WORLD";
	const float SCALE = 32.f;
	const float PI = 3.141592653589793f;
}

class Utility
{
private:
	Utility() {};
	~Utility() {};

public:
	static float DEGTORAD(float degrees) { return (degrees * ut::PI) / 180; };
	static float RADTODEG(float radians) { return (radians * 180) / ut::PI; };

	static float ScaleToB2(float number) { return number / ut::SCALE; };
	static float ScaleFromB2(float number) { return number * ut::SCALE; };

	static b2Vec2 SFVECtoB2VEC(sf::Vector2f vector, bool scale = false) { return b2Vec2(vector.x / (scale ? ut::SCALE : 1.f), vector.y / (scale ? ut::SCALE : 1.f)); };
	static sf::Vector2f B2VECtoSFVEC(b2Vec2 vector, bool scale = false) { return sf::Vector2f(vector.x * (scale ? ut::SCALE : 1.f), vector.y * (scale ? ut::SCALE : 1.f)); };
};

#endif
