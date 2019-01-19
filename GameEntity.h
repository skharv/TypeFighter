#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "Utility.h"

using namespace std;
using namespace sf;

class GameEntity
{
protected:
	b2Body* body;

	Vector2f position;

	virtual void CreateBody(b2World &World, Vector2f Size);
	virtual void CreateCircularBody(b2World &World, float Radius);
	virtual void CreateRoundedFixture(Vector2f Size);
	virtual void CreateSquareFixture(Vector2f Size);
	virtual void CreateCircleFixture(float Radius);
public:
	virtual void Update() {};
	virtual void Update(vector<GameEntity*> &Entities) {};
	virtual void Draw(RenderWindow & Window) {};

	void SetPosition(Vector2f Position) { position = Position; body->SetTransform(Utility::SFVECtoB2VEC(position, true), 0); };
	Vector2f GetPosition() { return Utility::B2VECtoSFVEC(body->GetPosition(), true); };
	b2Body* GetBody() { return body; };

	GameEntity();
	~GameEntity();
};

#endif