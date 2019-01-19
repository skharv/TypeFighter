#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

#include "Character.h"
#include "Attack.h"
#include "TypingControls.h"
#include "SplitAnimationManager.h"

#define ROLLDURATION 20
#define ROLLDISTANCE 250.0f
#define ROLLSIZE 16.0f
#define ROUNDING 2.5f
#define STANDSIZE 24.0f
#define JUMPHEIGHT 500.0f
#define JUMPDISTANCE 250.0f
#define JUMPDURATION 10
#define RUNSPEED 50.0f

class Controllable : public Character
{
private:
	bool pushing = false;
	bool jumping = false;
	bool roll = false;
	bool killable = false;
	bool run = false;
	bool die = false;
	bool act = false;
	bool standing = false;
	bool shoot = false;
	int rollCounter = ROLLDURATION;
	int jumpCounter = JUMPDURATION;
	int runAddition = 0;
	int standCounter = 0;
	string action;
	Vector2f oldPosition;

	SoundBuffer JumpBuffer, RollBuffer, ShootBuffer, AttackBuffer;
	Sound JumpSound, RollSound, ShootSound, AttackSound;

	Attack projectile;
	Attack melee;
	SplitAnimationManager animations;

	void Roll();
	void Stand();
public:
	void Update(vector<GameEntity*> &Entities);
	void Draw(RenderWindow &Window);
	void ProcessInput(TypingControls &Controls);
	bool GetAct() { return act; };
	Vector2f GetVelocity() { return velocity; };
	int GetStandCounter() { return standCounter; };

	Controllable(b2World &World);
	Controllable();
	~Controllable();
};

#endif
