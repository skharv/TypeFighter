#ifndef HITBOX_H
#define HITBOX_H

#include "GameEntity.h"
#include "AnimationHandler.h"

using namespace std;

class HitBox : public GameEntity
{
private:
	int timeToLive; 
	float rotation = 0.0f;
	bool hasAnimation = false;
	AnimationHandler animation;

public:
	void Update(vector<GameEntity*> &Entities);
	void Draw(RenderWindow& Window);

	void SetAnimation(String FilePath, Vector2i Frames);
	AnimationHandler GetAnimation() { return animation; };
		
	HitBox();
	HitBox(b2Body * Owner, float Radius, Vector2f Position, Vector2f Velocity, bool Gravity, int TimeToLive, string filePath = "");
	~HitBox();
};

#endif