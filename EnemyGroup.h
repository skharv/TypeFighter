#ifndef ENEMYGROUP_H
#define ENEMYGROUP_H

#include <SFML/Graphics.hpp>
#include <vector>;
#include "NPC.h"

using namespace std;
using namespace sf;

class EnemyGroup
{
private:
public:
	NPC* Spawn(b2World &World, Vector2f SpawnPoint, int Classification);

	EnemyGroup();
	~EnemyGroup();
};

#endif