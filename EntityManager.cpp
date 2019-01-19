#include "EntityManager.h"

void EntityManager::Update(Vector2f KillSpot)
{
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->Update(entities);
			if (entities[i]->GetPosition().x < KillSpot.x)
				entities[i]->GetBody()->SetUserData((void*)ut::KILL);
			if (entities[i]->GetPosition().y > KillSpot.y)
				entities[i]->GetBody()->SetUserData((void*)ut::KILL);
		}
}

void EntityManager::Draw(RenderWindow & Window)
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Draw(Window);
	}
}

void EntityManager::PopulateDead(vector<b2Body*>& Vector)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetBody()->GetUserData() == ut::KILL)
		{
			Vector.push_back(entities[i]->GetBody());
			delete entities[i];
			entities.erase(entities.begin() + i);

			if (entities.size() == 0)
				break;
		}
	}
}

void EntityManager::KillDying(vector<b2Body*>& Vector)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetBody()->GetUserData() == ut::DIE)
		{
			Vector.push_back(entities[i]->GetBody());
			delete entities[i];
			entities.erase(entities.begin() + i);

			if (entities.size() == 0)
				break;
		}
	}
}

void EntityManager::KillAll(vector<b2Body*>& Vector)
{
	int j = entities.size();
	for (int i = 0; i < j; i++)
	{
		Vector.push_back(entities[0]->GetBody());
		delete entities[0];
		entities.erase(entities.begin() );

		if (entities.size() == 0)
			break;
	}
}

EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}
