#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameEntity.h"
#include "AnimationHandler.h"
#include "SFML\Audio.hpp"

class Character : public GameEntity
{
protected:
	Vector2f velocity, desiredVelocity;
	AnimationHandler* animation;

public:
	void SetVelocity(Vector2f Velocity) { desiredVelocity = Velocity; };
	Character();
	~Character();
};

#endif