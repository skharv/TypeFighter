#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <Box2D/Box2D.h>
#include "utility.h"

class CollisionHandler : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	CollisionHandler();
	~CollisionHandler();
};

#endif
