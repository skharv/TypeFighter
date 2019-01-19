#include "Attack.h"

HitBox* Attack::Spawn()
{
	HitBox* hitbox = new HitBox(owner, radius, position, velocity, gravity, hitBoxDuration);
	if (hitBoxDuration <= 0)
	{
		hitbox->SetAnimation(animationFilePath, animationFrames);
		hitbox->GetAnimation().SetOrigin(projectileOffset);
	}

	if (gravityScale != 1.0f)
		hitbox->GetBody()->SetGravityScale(gravityScale);
	return hitbox;
}

void Attack::Update(vector<GameEntity*> &Entities)
{
	counter--;
	delayCounter--;

	if(delayCounter <= 0)
	{
		if (attacksRemaining > 0)
		{
			if (spawn)
			{
				Entities.push_back(Spawn());
				spawn = false;
				attacksRemaining--;
			}

			if (counter <= 0 && !spawn)
			{
				spawn = true;
				counter = interval;
			}
		}
	}

	animation.Update(position + Utility::B2VECtoSFVEC(owner->GetPosition(), true));
}

void Attack::Draw(RenderWindow & Window)
{
	animation.Draw(Window);
}

void Attack::Action()
{
	delayCounter = delay;
	attacksRemaining = numberOfAttacks;
	counter = interval;
	spawn = true;
	animation.SetCurrentFrame(0);
}

Attack::Attack(b2Body * Owner, float Radius, Vector2f Position, Vector2f Velocity, int Interval, int NumberOfAttacks, int HitBoxDuration, int Delay, bool Gravity, string FilePath, Vector2i AnimationFrames)
{
	owner = Owner;
	radius = Radius;
	position = Position;
	velocity = Velocity;
	interval = Interval;
	numberOfAttacks = NumberOfAttacks;
	hitBoxDuration = HitBoxDuration;
	delay = Delay;
	gravity = Gravity;
	spawn = false;
	animationFilePath = FilePath;
	animationFrames = AnimationFrames;
	animation = AnimationHandler();
}

Attack::Attack()
{
}


Attack::~Attack()
{
}
