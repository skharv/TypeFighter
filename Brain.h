#ifndef BRAIN_H
#define BRAIN_H

#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "Utility.h"
#include "RayCastCallback.h"

#define RAYLENGTH 250
#define RAYDIRECTION 270
#define PROJECTILECOUNTER 300
#define JUMPCOUNTER 10
#define ANGLE 0.75

using namespace sf;

class Brain
{
private:
	float rayAngle = Utility::DEGTORAD(RAYDIRECTION);
	float rayLength = RAYLENGTH;
	b2RayCastInput input;
	b2Vec2 intersectionPoint;
	Vector2f spawnPoint;

	bool countUp = true;
	bool action = false;
	bool move = false;
	float rockAngle = 0;
	int actionCounter = 0;
	float actionTimer = PROJECTILECOUNTER;
	int moveCounter = 0;

	int classification;
public:
	void Update(b2Body &Body, Vector2f CurrentPosition = Vector2f(0, 0));
	void Draw(RenderWindow &Window);
	bool GetAction(int i) { return i == 0 ?  action : move; };

	Brain(int Category, Vector2f SpawnPoint);

	Brain();
	~Brain();
};

#endif