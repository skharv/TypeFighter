#include "NPC.h"

b2Body* NPC::Spawn(b2World &World, Vector2f Position, int Classification)
{
	CreateBody(World, size);
	body->SetTransform(Utility::SFVECtoB2VEC(Position, true), 0);
	body->SetUserData((void*)ut::ENEMY);
	classification = Classification;
	SpawnPoint = Position;
	vector<int> _frames;
	vector<AnimationHandler::frameIndex>* _animations = new vector<AnimationHandler::frameIndex>();

	int _a;
	string _filePath;

	switch (classification)
	{
	case 0:
		body->GetFixtureList()->SetFriction(1.0f);
		brain = Brain(classification, SpawnPoint);
		action = Attack(body, 2.5f, Vector2f(-20, 0), Vector2f(-0.75, 0), 0, 1, 0, 30, false, "Images/Fireball.png", Vector2i(4, 1));
		action.SetProjectileOffset(Vector2f(16, 0));
		//----------ANIMATIONS----------//
		animation = new AnimationHandler("Images/FlameThrower.png", Vector2i(6, 2));
		animation->SetOrigin(Vector2f(16, 20));
		_frames.push_back(0);
		_frames.push_back(1);
		_frames.push_back(2);
		animation->CreateAnimation("Default", _frames, true, false);
		animation->SetCurrentAnimation("Default");
		animation->SetDefaultAnimation("Default");
		_frames.clear();
		_frames.push_back(3);
		_frames.push_back(3);
		_frames.push_back(4);
		_frames.push_back(4);
		_frames.push_back(5);
		_frames.push_back(5);
		animation->CreateAnimation("Fire", _frames, false, false);
		_frames.clear();
		_frames.push_back(6);
		_frames.push_back(7);
		_frames.push_back(8);
		_frames.push_back(9);
		_frames.push_back(10);
		_frames.push_back(11);
		animation->CreateAnimation("Die", _frames, false, false);

		//----------SOUNDS----------//
		_a = (rand() % 6) + 1;
		_filePath.append("Sounds/Creature");
		_filePath.append(to_string(_a));
		_filePath.append(".ogg");
		DeathBuffer.loadFromFile(_filePath);
		DeathSound.setBuffer(DeathBuffer);
		break;
	case 1:
		body->SetGravityScale(0.1f);
		body->GetFixtureList()->SetFriction(0.0f);
		brain = Brain(classification, SpawnPoint);
		action = Attack(body, 2.5f, Vector2f(0, 25), Vector2f(0, 0), 0, 1, 0, 30, true, "Images/Rock.png", Vector2i(1, 1));
		action.SetGravityScale(0.25f);

		//-------START BODY ANIMATIONS-------//
		animations.LoadLeader("Images/FlapperWings.png", Vector2i(5, 2));
		_animations->push_back(AnimationHandler::frameIndex(4));
		animations.GetLeader().animHandler->CreateAnimation("Default", *_animations, true, false);
		animations.GetLeader().animHandler->SetCurrentAnimation("Default");
		animations.GetLeader().animHandler->SetDefaultAnimation("Default");
		_animations->clear();

		_animations->push_back(AnimationHandler::frameIndex(0, Vector2f(0, -1)));
		_animations->push_back(AnimationHandler::frameIndex(1));
		_animations->push_back(AnimationHandler::frameIndex(2, Vector2f(0, -1)));
		_animations->push_back(AnimationHandler::frameIndex(3));
		animations.GetLeader().animHandler->CreateAnimation("Flap", *_animations, false, false);
		_animations->clear();

		_animations->push_back(AnimationHandler::frameIndex(5));
		_animations->push_back(AnimationHandler::frameIndex(6));
		_animations->push_back(AnimationHandler::frameIndex(7));
		_animations->push_back(AnimationHandler::frameIndex(8));
		_animations->push_back(AnimationHandler::frameIndex(9));
		animations.GetLeader().animHandler->CreateAnimation("Die", *_animations, false, false);
		_animations->clear();
		//--------END BODY ANIMATIONS--------//

		//-------START WING ANIMATIONS-------//
		animations.LoadFollower("Images/FlapperBody.png", Vector2i(5, 2), Vector2f(0, 0));
		_animations->push_back(AnimationHandler::frameIndex(0));
		animations.GetFollower(0).animHandler->CreateAnimation("Default", *_animations, true, false);
		animations.GetFollower(0).animHandler->SetCurrentAnimation("Default");
		animations.GetFollower(0).animHandler->SetDefaultAnimation("Default");
		_animations->clear();

		_animations->push_back(AnimationHandler::frameIndex(1));
		_animations->push_back(AnimationHandler::frameIndex(2));
		_animations->push_back(AnimationHandler::frameIndex(3));
		_animations->push_back(AnimationHandler::frameIndex(4));
		animations.GetFollower(0).animHandler->CreateAnimation("Throw", *_animations, false, false);
		_animations->clear();

		_animations->push_back(AnimationHandler::frameIndex(5));
		_animations->push_back(AnimationHandler::frameIndex(6));
		_animations->push_back(AnimationHandler::frameIndex(7));
		_animations->push_back(AnimationHandler::frameIndex(8));
		_animations->push_back(AnimationHandler::frameIndex(9));
		animations.GetFollower(0).animHandler->CreateAnimation("Die", *_animations, false, false);
		_animations->clear();
		//--------END WING ANIMATIONS--------//

		_a = (rand() % 3) + 1;
		_filePath.append("Sounds/Screech");
		_filePath.append(to_string(_a));
		_filePath.append(".ogg");
		DeathBuffer.loadFromFile(_filePath);
		DeathSound.setBuffer(DeathBuffer);

		FlapBuffer.loadFromFile("Sounds/Flap5.ogg");
		FlapSound.setBuffer(FlapBuffer);
		break;
	case 2:
		body->GetFixtureList()->SetFriction(1.0f);
		body->SetUserData((void*)ut::INVULNERABLE);
		brain = Brain(classification, SpawnPoint);
		action = Attack(body, 2.5f, Vector2f(-20, 0), Vector2f(0, 0), 0, 1, 0, 0, false, "Images/Point.png", Vector2i(1, 1));
		//----------ANIMATIONS----------//
		animation = new AnimationHandler("Images/SpiderAnim.png", Vector2i(5, 1));
		animation->SetOrigin(Vector2f(32, 36));
		_frames.push_back(0);
		_frames.push_back(1);
		_frames.push_back(2);
		_frames.push_back(3);
		_frames.push_back(4);
		animation->CreateAnimation("Default", _frames, true, false);
		animation->SetCurrentAnimation("Default");
		animation->SetDefaultAnimation("Default");
		_frames.clear();
		_frames.push_back(0);
		animation->CreateAnimation("Die", _frames, false, false);
		_frames.clear();
		//----------SOUNDS----------//
		break;
	default:
		break;
	}

	DeathSound.setVolume(75);

	return body;
}

void NPC::Update(vector<GameEntity*> &Entities)
{
	brain.Update(*body, position);

	switch(classification)
	{
	case 0:
		if (brain.GetAction(0) && body->GetUserData() != (void*)ut::DIE)
		{
			action.Action();
			animation->SetCurrentAnimation("Fire");
			animation->SetLoop(false);
		}

		if (animation->GetCurrentAnimation() == "Default" && die)
			body->SetUserData((void*)ut::KILL);

		if (body->GetUserData() == (void*)ut::DIE && !die)
		{
			b2Filter a;
			a.maskBits = 0x0000;
			body->GetFixtureList()->SetFilterData(a);
			body->SetGravityScale(0.0f);
			animation->SetCurrentAnimation("Die");
			die = true;
			DeathSound.play();
		}

		animation->Update(Utility::B2VECtoSFVEC(body->GetPosition(), true));

		break;
	case 1:
		if(body->GetUserData() != (void*)ut::DIE)
		{
			if (brain.GetAction(0))
			{
				if (animations.GetFollower(0).animHandler->GetCurrentAnimation() != "Throw")
					animations.GetFollower(0).animHandler->SetCurrentAnimation("Throw");
				action.Action();
			}
			if (brain.GetAction(1))
			{
				if (Utility::B2VECtoSFVEC(body->GetPosition(), true).y > SpawnPoint.y)
				{
					if (animations.GetLeader().animHandler->GetCurrentAnimation() != "Flap")
					{
						animations.GetLeader().animHandler->SetCurrentAnimation("Flap");
						FlapSound.play();
					}

					body->ApplyLinearImpulseToCenter(Utility::SFVECtoB2VEC(Vector2f(0, -1)), true);

					if (Utility::B2VECtoSFVEC(body->GetPosition(), true).x > SpawnPoint.x)
						body->ApplyLinearImpulseToCenter(Utility::SFVECtoB2VEC(Vector2f(((rand() % 2) - 2) * 0.3, 0)), true);
					else if (Utility::B2VECtoSFVEC(body->GetPosition(), true).x < SpawnPoint.x)
						body->ApplyLinearImpulseToCenter(Utility::SFVECtoB2VEC(Vector2f(((rand() % 2)) * 0.3, 0)), true);
					else
						body->ApplyLinearImpulseToCenter(Utility::SFVECtoB2VEC(Vector2f(((rand() % 3) - 1) * 0.3, 0)), true);
				}
			}
		}

		if (animations.GetLeader().animHandler->GetCurrentAnimation() == "Default" && die)
			body->SetUserData((void*)ut::KILL);

		if (body->GetUserData() == (void*)ut::DIE && !die)
		{
			b2Filter a;
			a.maskBits = 0x0000;
			body->GetFixtureList()->SetFilterData(a);
			body->SetGravityScale(0.0f);
			animations.SetAnimation("Die");
			die = true;
			DeathSound.play();
		}

		animations.Update(Utility::B2VECtoSFVEC(body->GetPosition(), true));
		break;
	case 2:
		if (brain.GetAction(0) && body->GetUserData() != (void*)ut::DIE)
		{
			action.Action();
			animation->SetCurrentAnimation("Default");
			animation->SetLoop(false);
		}

		if (animation->GetCurrentAnimation() == "Default" && die)
			body->SetUserData((void*)ut::KILL);

		if (body->GetUserData() == (void*)ut::DIE && !die)
		{
			b2Filter a;
			a.maskBits = 0x0000;
			body->GetFixtureList()->SetFilterData(a);
			body->SetGravityScale(0.0f);
			animation->SetCurrentAnimation("Die");
			die = true;
			DeathSound.play();
		}

		body->SetLinearVelocity(Utility::SFVECtoB2VEC(Vector2f(-1, 0)));

		animation->Update(Utility::B2VECtoSFVEC(body->GetPosition(), true));
		break;
	default:
		break;
	}

	action.Update(Entities);
}

void NPC::Draw(RenderWindow & Window)
{
	switch (classification)
	{
	case 0:
		if (!(animation->GetCurrentAnimation() == "Default" && die))
			animation->Draw(Window);
		break;
	case 1:
		if (!(animations.GetLeader().animHandler->GetCurrentAnimation() == "Default" && die))
			animations.Draw(Window);
		break;
	case 2:
		if (!(animation->GetCurrentAnimation() == "Default" && die))
			animation->Draw(Window);
		break;
	default:
		break;
	}

	//brain.Draw(Window);
}

NPC::NPC(Vector2f Size)
{
	size = Size;
	brain = Brain();
}

NPC::NPC()
{
}


NPC::~NPC()
{
}
