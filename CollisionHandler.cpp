#include "CollisionHandler.h"

void CollisionHandler::BeginContact(b2Contact * contact)
{
	b2Fixture* _fixtureA = contact->GetFixtureA();
	b2Fixture* _fixtureB = contact->GetFixtureB();

	if (_fixtureA->GetBody()->GetUserData() == (void*)ut::HITBOX || _fixtureA->GetBody()->GetUserData() == (void*)ut::BULLET)
	{
		if (_fixtureB->GetBody()->GetUserData() == (void*)ut::ENEMY || _fixtureB->GetBody()->GetUserData() == (void*)ut::PLAYER || _fixtureB->GetBody()->GetUserData() == (void*)ut::DESTRUCTABLE)
		{
			if (!_fixtureB->IsSensor())
			{
				if (_fixtureB->GetBody()->GetUserData() == (void*)ut::ENEMY)
				{
					if (_fixtureA->GetBody()->GetUserData() == (void*)ut::HITBOX)
						lastDeathType = MELEE;
					else if (_fixtureA->GetBody()->GetUserData() == (void*)ut::BULLET)
						lastDeathType = BULLET;
					else
						lastDeathType = NONE;
				}

				_fixtureA->GetBody()->SetUserData((void*)ut::KILL);
				_fixtureB->GetBody()->SetUserData((void*)ut::DIE);

				justKilled = true;
			}
		}

		if (_fixtureB->GetBody()->GetUserData() == (void*)ut::INVULNERABLE)
			_fixtureA->SetRestitution(1.0f);

		if (_fixtureB->GetBody()->GetUserData() == (void*)ut::WORLD)
			_fixtureA->GetBody()->SetUserData((void*)ut::KILL);
	}
	else if (_fixtureB->GetBody()->GetUserData() == (void*)ut::HITBOX || _fixtureB->GetBody()->GetUserData() == (void*)ut::BULLET)
	{
		if (_fixtureA->GetBody()->GetUserData() == (void*)ut::ENEMY || _fixtureA->GetBody()->GetUserData() == (void*)ut::PLAYER || _fixtureA->GetBody()->GetUserData() == (void*)ut::DESTRUCTABLE)
		{
			if (!_fixtureA->IsSensor())
			{
				if (_fixtureA->GetBody()->GetUserData() == (void*)ut::ENEMY)
				{
					if (_fixtureB->GetBody()->GetUserData() == (void*)ut::HITBOX)
						lastDeathType = MELEE;
					else if (_fixtureB->GetBody()->GetUserData() == (void*)ut::BULLET)
						lastDeathType = BULLET;
					else
						lastDeathType = NONE;
				}

				_fixtureA->GetBody()->SetUserData((void*)ut::DIE);
				_fixtureB->GetBody()->SetUserData((void*)ut::KILL);

				justKilled = true;
			}
		}

		if (_fixtureA->GetBody()->GetUserData() == (void*)ut::INVULNERABLE)
			_fixtureB->SetRestitution(1.0f);

		if (_fixtureA->GetBody()->GetUserData() == (void*)ut::WORLD)
			_fixtureB->GetBody()->SetUserData((void*)ut::KILL);
	}
}

void CollisionHandler::EndContact(b2Contact * contact)
{
}

void CollisionHandler::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
}

void CollisionHandler::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
}

CollisionHandler::CollisionHandler()
{
}

CollisionHandler::~CollisionHandler()
{
}
