#include "EnemyGroup.h"

NPC* EnemyGroup::Spawn(b2World &World, Vector2f SpawnPoint, int Classification)
{
	NPC* _entity = new NPC();

	switch (Classification)
	{
	case 0:
		_entity = new NPC(Vector2f(16.0f, 24.0f));
		_entity->Spawn(World, SpawnPoint, Classification);
		break;
	case 1:
		_entity = new NPC(Vector2f(16.0f, 16.0f));
		_entity->Spawn(World, SpawnPoint, Classification);
		break;
	case 2:
		_entity = new NPC(Vector2f(36.0f, 24.0f));
		_entity->Spawn(World, SpawnPoint, Classification);
		break;
	default:
		break;
	}

	return _entity;
}

EnemyGroup::EnemyGroup()
{
}


EnemyGroup::~EnemyGroup()
{
}
