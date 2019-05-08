#include "HitBox.h"

void HitBox::Update(vector<GameEntity*> &Entities)
{
	Vector2f _oldPosition = position;
	if (timeToLive > 0)
	{		
		if (--timeToLive == 0)
		{
			body->SetUserData((void*)ut::KILL);
		}
	}

	position = Utility::B2VECtoSFVEC(body->GetPosition(), true);

	float _yChange = _oldPosition.y - position.y;
	float _xChange = _oldPosition.x - position.x;

	rotation = atan2(_yChange, _xChange);

	if(hasAnimation)
	{
		animation.Update(position);
		animation.SetRotation((rotation * 180 / ut::PI));
	}
}

void HitBox::Draw(RenderWindow & Window)
{
	if(hasAnimation)
		animation.Draw(Window);
}

void HitBox::SetAnimation(String FilePath, Vector2i Frames)
{
	hasAnimation = true;

	vector<int>* _animations = new vector<int>();

	animation = AnimationHandler(FilePath, Frames);

	for (int i = 0; i < Frames.x * Frames.y; i++)
	{
		_animations->push_back(i);
	}

	animation.CreateAnimation("Default", *_animations, true, false);
	animation.SetDefaultAnimation("Default");
}

HitBox::HitBox()
{
}

HitBox::HitBox(b2Body* Owner, float Radius, Vector2f Position, Vector2f Velocity, bool Gravity, int TimeToLive, string filePath)
{
	CreateCircularBody(*Owner->GetWorld(), Utility::ScaleToB2(Radius));
	body->SetTransform(Owner->GetPosition() + Utility::SFVECtoB2VEC(Position, true), true);
	body->ApplyLinearImpulseToCenter(Utility::SFVECtoB2VEC(Velocity, true), true);

	body->SetBullet(true);
	if (Velocity == Vector2f(0, 0))
		body->SetUserData((void*)ut::HITBOX);
	else
		body->SetUserData((void*)ut::BULLET);

	if(Gravity)
		body->SetGravityScale(1.0f);
	else
		body->SetGravityScale(0.0f);

	timeToLive = TimeToLive;

	if (timeToLive > 0)
		body->GetFixtureList()->SetSensor(true);
	else
		body->GetFixtureList()->SetSensor(false);



}


HitBox::~HitBox()
{
}
