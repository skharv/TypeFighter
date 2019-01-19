#include "Destructable.h"

b2Body* Destructable::Spawn(b2World &World, Vector2f Position, int Classification)
{
	CreateBody(World, size);
	body->SetTransform(Utility::SFVECtoB2VEC(Position, true), 0);
	body->SetUserData((void*)ut::DESTRUCTABLE);
	body->SetFixedRotation(false);
	position = Position;
	classification = Classification;
	vector<int> _frames;

	//int variation = rand() % 4;

	switch (classification)
	{
	case 0:
		body->GetFixtureList()->SetFriction(1.0f);
		animation = new AnimationHandler("Images/Crates1.png", Vector2i(5, 1));
		_frames.push_back(0);
		animation->CreateAnimation("Default", _frames, false, true);
		animation->SetDefaultAnimation("Default");
		_frames.clear();
		_frames.push_back(1);
		_frames.push_back(2);
		_frames.push_back(3);
		_frames.push_back(4);
		animation->CreateAnimation("Die", _frames, false, false);
		break;
	case 1:
		body->SetType(b2BodyType::b2_staticBody);
		body->SetTransform(body->GetPosition() - Utility::SFVECtoB2VEC(Vector2f(0, 4), true), 0.0f);
		animation = new AnimationHandler("Images/Brick.png", Vector2i(1, 1));
		_frames.push_back(0);
		animation->CreateAnimation("Default", _frames, false, true);
		animation->SetDefaultAnimation("Default");
		break;
	default:
		break;
	}

	return body;
}

void Destructable::Draw(RenderWindow & Window)
{
	if (!(animation->GetCurrentAnimation() == "Default" && die))
		animation->Draw(Window);
}

void Destructable::Update(vector<GameEntity*> &Entities)
{
	switch (classification)
	{
	case 0:
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
		}
		break;
	case 1:
		break;
	default:
		break;
	}

	animation->Update(Utility::B2VECtoSFVEC(body->GetPosition(), true));
	animation->SetRotation(Utility::RADTODEG(body->GetAngle()));
}

Destructable::Destructable(Vector2f Size)
{
	size = Size;
}

Destructable::Destructable()
{

}

Destructable::~Destructable()
{
}
