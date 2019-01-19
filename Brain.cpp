#include "Brain.h"

void Brain::Update(b2Body &Body, Vector2f CurrentPosition)
{
	b2Vec2 previousIntersectionPoint = intersectionPoint;
	float closestFraction = 1;
	actionCounter--;
	moveCounter--;
	action = false;
	move = false;

	switch (classification)
	{
	case 0:
		break;
	case 1:
		if (countUp)
			rockAngle += Utility::DEGTORAD(0.5);
		else
			rockAngle -= Utility::DEGTORAD(0.5);

		if (moveCounter <= 0)
		{
			move = true;
			moveCounter = JUMPCOUNTER;
		}
		break;
	case 2:
		break;
	default:
		break;
	}

	input.p1 = Body.GetPosition();
	input.p2 = input.p1 + Utility::SFVECtoB2VEC(Vector2f(sinf(rayAngle - rockAngle)*rayLength, cosf(rayAngle - rockAngle)*rayLength), true);
	input.maxFraction = 1;
	RayCastCallback callback;
	Body.GetWorld()->RayCast(&callback, input.p1, input.p2);

	if (callback.hit)
	{
		if ((callback.Body->GetUserData() == ut::PLAYER || callback.Body->GetUserData() == ut::DESTRUCTABLE) && actionCounter <= 0)
		{
			action = true;
			actionCounter = actionTimer;
		}
	}

	intersectionPoint = input.p1 + closestFraction * (input.p2 - input.p1);

	float oldDist = sqrt(((Body.GetPosition().x - previousIntersectionPoint.x) * (Body.GetPosition().x - previousIntersectionPoint.x)) + ((Body.GetPosition().y - previousIntersectionPoint.y) * (Body.GetPosition().y - previousIntersectionPoint.y)));
	float newDist = sqrt(((Body.GetPosition().x - intersectionPoint.x) * (Body.GetPosition().x - intersectionPoint.x)) + ((Body.GetPosition().y - intersectionPoint.y) * (Body.GetPosition().y - intersectionPoint.y)));

	if (newDist > oldDist)
		countUp = false;
	else
		countUp = true;
}

void Brain::Draw(RenderWindow & Window)
{
	Vertex line[]
	{
		Vertex(Utility::B2VECtoSFVEC(input.p1, true)),
		Vertex(Utility::B2VECtoSFVEC(intersectionPoint, true))
	};
	switch (classification)
	{
	case 0:
		Window.draw(line, 2, sf::Lines);
		break;
	case 1:
		Window.draw(line, 2, sf::Lines);
		break;
	case 2:
		Window.draw(line, 2, sf::Lines);
		break;
	default:
		break;
	}
}

Brain::Brain(int Classification, Vector2f SpawnPoint)
{
	spawnPoint = SpawnPoint;

	classification = Classification;
	switch (classification)
	{
	case 0:
		rayAngle = Utility::DEGTORAD(RAYDIRECTION);
		break;
	case 1:
		rockAngle = Utility::DEGTORAD(30);
		rayAngle = Utility::DEGTORAD(rockAngle);
		break;
	case 2:
		rayAngle = Utility::DEGTORAD(RAYDIRECTION);
		rayLength = 30;
		actionTimer = 1;
		break;
	default:
		break;
	}
}

Brain::Brain()
{
}


Brain::~Brain()
{
}
