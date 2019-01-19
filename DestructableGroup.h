#ifndef DESTRUCTABLEGROUP_H
#define DESTRUCTABLEGROUP_H

#include <SFML/Graphics.hpp>
#include <vector>;
#include "Destructable.h"

using namespace std;
using namespace sf;

class DestructableGroup
{
private:
public:
	Destructable* Spawn(b2World &World, Vector2f SpawnPoint, int Classification);

	DestructableGroup();
	~DestructableGroup();
};

#endif