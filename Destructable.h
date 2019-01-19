#ifndef DESTRUCTABLE_H
#define DESTRUCTABLE_H

#include "Character.h"

class Destructable :
	public Character
{
private:
	Vector2f size, position;
	bool die = false;
	float rotation;
	int classification;

public:
	b2Body * Spawn(b2World &World, Vector2f Position, int Classification);

	void Draw(RenderWindow &Window);
	void Update(vector<GameEntity*> &Entities);

	Destructable(Vector2f Size);
	Destructable();
	~Destructable();
};

#endif