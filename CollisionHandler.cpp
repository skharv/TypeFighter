#include "CollisionHandler.h"

void CollisionHandler::BeginContact(b2Contact * contact)
{
	b2Fixture* _fixtureA = contact->GetFixtureA();
	b2Fixture* _fixtureB = contact->GetFixtureB();

	if (_fixtureA->GetBody()->GetUserData() == (void*)ut::HITBOX)
	{
		if (_fixtureB->GetBody()->GetUserData() == (void*)ut::ENEMY || _fixtureB->GetBody()->GetUserData() == (void*)ut::PLAYER || _fixtureB->GetBody()->GetUserData() == (void*)ut::DESTRUCTABLE)
		{
			if (!_fixtureB->IsSensor())
			{
				_fixtureA->GetBody()->SetUserData((void*)ut::KILL);
				_fixtureB->GetBody()->SetUserData((void*)ut::DIE);
				
			}
		}

		if (_fixtureB->GetBody()->GetUserData() == (void*)ut::INVULNERABLE)
			_fixtureA->SetRestitution(1.0f);

		if (_fixtureB->GetBody()->GetUserData() == (void*)ut::WORLD)
			_fixtureA->GetBody()->SetUserData((void*)ut::KILL);
	}
	else if (_fixtureB->GetBody()->GetUserData() == (void*)ut::HITBOX)
	{
		if (_fixtureA->GetBody()->GetUserData() == (void*)ut::ENEMY || _fixtureA->GetBody()->GetUserData() == (void*)ut::PLAYER || _fixtureA->GetBody()->GetUserData() == (void*)ut::DESTRUCTABLE)
		{
			if (!_fixtureA->IsSensor())
			{
				_fixtureA->GetBody()->SetUserData((void*)ut::DIE);
				_fixtureB->GetBody()->SetUserData((void*)ut::KILL);
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
