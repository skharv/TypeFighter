#ifndef NPC_H
#define NPC_H

#include "Character.h"
#include "SplitAnimationManager.h"
#include "Brain.h"
#include "Attack.h"

#define ROUNDING 2.5f
#define MOVESPEED 75.0f

class NPC :	public Character
{
private:
	Brain brain;
	Vector2f SpawnPoint;
	Attack action;

	bool die = false;

	SoundBuffer DeathBuffer;
	Sound DeathSound;
	SoundBuffer FlapBuffer;
	Sound FlapSound;

	SplitAnimationManager animations;

	int classification;

	Vector2f size;
public:
	b2Body* Spawn(b2World &World, Vector2f Position, int Classification);

	void Update(vector<GameEntity*> &Entities);
	void Draw(RenderWindow &Window);

	NPC(Vector2f Size);
	NPC();
	~NPC();
};

#endif

