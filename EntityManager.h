#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "GameEntity.h"
#include <vector>

using namespace std;
using namespace sf;

class EntityManager
{
private:
	vector<GameEntity*> entities;

	//when killing now, we want to instead add them to a list of dying entities.
	//then we will have them play their death animation and remove all collision.
	// at the end of their death animation, they will then be marked as Kill
	vector<GameEntity*> dyingEntities;

public:
	void Update(Vector2f KillSpot);
	void Draw(RenderWindow &Window);

	void AddEntity(GameEntity* Entity) { entities.push_back(Entity); };
	vector<GameEntity*> GetEntities() { return entities; };
	void PopulateDead(vector<b2Body*> &Vector);
	void KillDying(vector<b2Body*> &Vector);
	void KillAll(vector<b2Body*> &Vector);

	EntityManager();
	~EntityManager();
};

#endif