#ifndef ATTACK_H
#define ATTACK_H

#include "HitBox.h"

using namespace std;

class Attack
{
private:
	Vector2f position, velocity, projectileOffset;
	Vector2i animationFrames;
	int interval = 0, counter = 0, numberOfAttacks = 0, attacksRemaining = 0, hitBoxDuration = 0, delay = 0, delayCounter = 0;
	float radius, gravityScale = 1.0f;
	bool gravity, spawn = false;
	string animationFilePath;

	b2Body* owner;

	AnimationHandler animation;
public:
	HitBox* Spawn();
	void Update(vector<GameEntity*> &Entities);
	void Draw(RenderWindow& Window);
	void SetGravityScale(float Scale) { gravityScale = Scale; };
	void SetProjectileOffset(Vector2f Offset) { projectileOffset = Offset; };
	void SetAnimation(AnimationHandler Animation) { animation = Animation; };
	AnimationHandler GetAnimation() { return animation; };
	void Action();

	Attack(b2Body* Owner, float Radius, Vector2f Position, Vector2f Velocity, int Interval, int NumberOfAttacks, int HitBoxDuration, int Delay, bool Gravity, string FilePath = "", Vector2i AnimationFrames = Vector2i(0, 0));
	Attack();
	~Attack();
};

#endif