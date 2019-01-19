#include "RayCastCallback.h"

RayCastCallback::RayCastCallback()
{
	hit = false;
};

float RayCastCallback::ReportFixture(b2Fixture * Fixture, const b2Vec2 & Point, const b2Vec2 & Normal, float Fraction)
{
	b2Body* _body = Fixture->GetBody();
	void* _userData = _body->GetUserData();
	if (_userData)
	{
		int index = *(int*)_userData;
		if (index == 0)
		{
			return -1.0f;
		}
	}

	hit = true;
	point = Point;
	normal = Normal;
	Body = _body;
	return Fraction;
}
