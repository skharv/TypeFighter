#include "DestructableGroup.h"

Destructable* DestructableGroup::Spawn(b2World &World, Vector2f SpawnPoint, int Classification)
{
	Destructable* _entity = new Destructable();

	switch (Classification)
	{
	case 0:
		_entity = new Destructable(Vector2f(16.0f, 16.0f));
		_entity->Spawn(World, SpawnPoint, Classification);
		break;
	case 1:
		_entity = new Destructable(Vector2f(16.0f, 8.0f));
		_entity->Spawn(World, SpawnPoint, Classification);
		break;
	default:
		break;
	}

	return _entity;
}

DestructableGroup::DestructableGroup()
{
}


DestructableGroup::~DestructableGroup()
{
}
