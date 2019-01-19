#include "GameEntity.h"



void GameEntity::CreateBody(b2World &World, Vector2f Size)
{
	b2BodyDef _bodyDef;
	_bodyDef.type = b2_dynamicBody;
	_bodyDef.position.Set(0, 0);
	_bodyDef.fixedRotation = true;
	body = World.CreateBody(&_bodyDef);
	CreateRoundedFixture(Size);
}

void GameEntity::CreateCircularBody(b2World & World, float Radius)
{
	b2BodyDef _bodyDef;
	_bodyDef.type = b2_dynamicBody;
	_bodyDef.position.Set(0, 0);
	body = World.CreateBody(&_bodyDef);
	CreateCircleFixture(Radius);
}

void GameEntity::CreateRoundedFixture(Vector2f Size)
{
	Vector2f _size = Size;
	Vector2f _halfSize = Vector2f(_size.x / 2, _size.y / 2);
	float _rounding = _halfSize.x / 4;

	b2Vec2 _vertices[8];
	_vertices[0].Set(-(_halfSize.x - _rounding) / ut::SCALE, -_halfSize.y / ut::SCALE);
	_vertices[1].Set((_halfSize.x - _rounding) / ut::SCALE, -_halfSize.y / ut::SCALE);
	_vertices[2].Set(_halfSize.x / ut::SCALE, -(_halfSize.y - _rounding) / ut::SCALE);
	_vertices[3].Set(_halfSize.x / ut::SCALE, (_halfSize.y - _rounding) / ut::SCALE);
	_vertices[4].Set((_halfSize.x - _rounding) / ut::SCALE, _halfSize.y / ut::SCALE);
	_vertices[5].Set(-(_halfSize.x - _rounding) / ut::SCALE, _halfSize.y / ut::SCALE);
	_vertices[6].Set(-_halfSize.x / ut::SCALE, (_halfSize.y - _rounding) / ut::SCALE);
	_vertices[7].Set(-_halfSize.x / ut::SCALE, -(_halfSize.y - _rounding) / ut::SCALE);

	b2PolygonShape _shape;
	_shape.Set(_vertices, 8);

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_shape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.1f;
	body->CreateFixture(&_fixtureDef);
}

void GameEntity::CreateSquareFixture(Vector2f Size)
{
	Vector2f _size = Size;
	Vector2f _halfSize = Vector2f(_size.x / 2, _size.y / 2);

	b2PolygonShape _shape;
	_shape.SetAsBox(_halfSize.x, _halfSize.y);

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_shape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.1f;
	body->CreateFixture(&_fixtureDef);
}

void GameEntity::CreateCircleFixture(float Radius)
{
	b2CircleShape _shape;
	_shape.m_radius = Radius;

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_shape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.1f;
	body->CreateFixture(&_fixtureDef);
}

GameEntity::GameEntity()
{
}


GameEntity::~GameEntity()
{
}
