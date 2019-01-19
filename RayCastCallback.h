#ifndef RAYCASTCALLBACK_H
#define RAYCASTCALLBACK_H

#include <Box2D\Box2D.h>

class RayCastCallback : public b2RayCastCallback
{
public:
	RayCastCallback();

	float ReportFixture(b2Fixture * Fixture, const b2Vec2 & Point, const b2Vec2 & Normal, float Fraction) override;

	bool hit;
	b2Vec2 point;
	b2Vec2 normal;
	b2Body* Body;
};

#endif