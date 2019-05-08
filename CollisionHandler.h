#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <Box2D/Box2D.h>
#include "utility.h"

enum DeathType
{
	NONE = 0,
	BULLET = 1,
	MELEE = 2
};

class CollisionHandler : public b2ContactListener
{
private:
	DeathType lastDeathType = NONE;
	bool justKilled = false;
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void ResetJustKilled() { justKilled = false; };
	DeathType GetLastDeathType() { return lastDeathType; };
	bool GetJustKilled() { return justKilled; };
	CollisionHandler();
	~CollisionHandler();
};

#endif
