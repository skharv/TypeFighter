#include "Controllable.h"

void Controllable::Update(vector<GameEntity*> &Entities)
{
	if (act)
	{
		if (action == "ATTACK")
		{
			animations.GetFollower(0).animHandler->SetCurrentAnimation("Swipe");
			melee.Action();
			AttackSound.play();
			runAddition += 5;
		}
		else if (action == "JUMP")
		{
			jumping = true;
			jumpCounter = JUMPDURATION;
			desiredVelocity.y = -JUMPHEIGHT;
			desiredVelocity.x = JUMPDISTANCE;
			JumpSound.play();
			runAddition += 5;
		}
		else if (action == "ROLL")
		{
			Roll();
			desiredVelocity.x = JUMPDISTANCE;
			RollSound.play();
			runAddition += 5;
		}
		else if (action == "SHOOT")
		{
			projectile.Action();
			animations.GetFollower(0).animHandler->SetCurrentAnimation("Release");
			shoot = false;
			ShootSound.play();
			runAddition += 5;
		}
		else
		{
			runAddition -= 5;
		}

		act = false;

	}

	if (jumping)
		jumpCounter--;

	if (jumpCounter <= 0)
	{
		for (b2Contact* contact = body->GetWorld()->GetContactList(); contact; contact = contact->GetNext())
		{
			if (contact->GetFixtureA() != nullptr && contact->GetFixtureB() != nullptr)
			{
				b2Fixture* _fixtureA = contact->GetFixtureA();
				b2Fixture* _fixtureB = contact->GetFixtureB();

				if (_fixtureA->GetBody() != body && _fixtureB->GetBody() != body)
					continue;

				if (contact->IsTouching())
					for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
					{
						if (f->IsSensor() && (_fixtureA == f || _fixtureB == f))
							jumping = false;
					}
			}
		}
	}

	if (!die)
	{
		for (b2Contact* contact = body->GetWorld()->GetContactList(); contact; contact = contact->GetNext())
		{
			if (contact->GetFixtureA() != nullptr && contact->GetFixtureB() != nullptr)
			{
				b2Fixture* _fixtureA = contact->GetFixtureA();
				b2Fixture* _fixtureB = contact->GetFixtureB();

				if (_fixtureA->GetBody() != body && _fixtureB->GetBody() != body)
					continue;

				if (contact->IsTouching())
					for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
					{
						if (f->IsSensor() && _fixtureA == f)
						{
							if (_fixtureB->GetBody()->GetUserData() == (void*)ut::ENEMY)
								animations.GetFollower(0).animHandler->SetCurrentAnimation("LongPush");
							else if (_fixtureB->GetBody()->GetUserData() == (void*)ut::DESTRUCTABLE)
								animations.GetFollower(0).animHandler->SetCurrentAnimation("ShortPush");

							pushing = true;
						}

						if (f->IsSensor() && _fixtureB == f)
						{
							if (_fixtureA->GetBody()->GetUserData() == (void*)ut::ENEMY)
								animations.GetFollower(0).animHandler->SetCurrentAnimation("LongPush");
							else if (_fixtureA->GetBody()->GetUserData() == (void*)ut::DESTRUCTABLE)
								animations.GetFollower(0).animHandler->SetCurrentAnimation("ShortPush");

							pushing = true;
						}
					}
			}
		}

		if (shoot)
			animations.GetFollower(0).animHandler->SetCurrentAnimation("Shoot");

		if (roll)
			rollCounter--;
		if (rollCounter <= 0 && roll)
			Stand();

		velocity = Utility::B2VECtoSFVEC(body->GetLinearVelocity(), true);

	}

	Vector2f* _impulse = new Vector2f(body->GetMass() * (desiredVelocity.x - velocity.x), body->GetMass() * (desiredVelocity.y - velocity.y));

	if (desiredVelocity.y == 0.0f || die)
		_impulse->y = 0.0f;
	if (desiredVelocity.x == 0.0f || die)
		_impulse->x = 0.0f;

	body->ApplyLinearImpulse(Utility::SFVECtoB2VEC(*_impulse, true), body->GetWorldCenter(), true);

	if(!die)
	{
		position = Utility::B2VECtoSFVEC(body->GetPosition(), true);

		if (position.x == oldPosition.x)
		{
			if(run)
			{
				if (animations.GetFollower(0).animHandler->GetCurrentAnimation() != "Swipe")
					animations.SetAnimation("Stand");
				else
					animations.GetLeader().animHandler->SetCurrentAnimation("Stand");

				standCounter++;
				
			}
		}
		else if (animations.GetLeader().animHandler->GetCurrentAnimation() == "Stand")
		{
			if (run)
			{
				animations.SetAnimation("Default");
				standCounter = 0;
			}
		}
	}

	//if (standCounter > 240)
	//	body->SetUserData((void*)ut::KILL);

	//Death stuff
	if (animations.GetFollower(0).animHandler->GetCurrentAnimation() == "Default" && die)
		body->SetUserData((void*)ut::KILL);

	if (body->GetUserData() == (void*)ut::DIE && !die)
	{
		b2Filter a;
		a.maskBits = 0x0000;
		b2Fixture*_fixture = body->GetFixtureList();
		while (_fixture->IsSensor())
			_fixture = _fixture->GetNext();
		_fixture->SetFilterData(a);
		body->SetGravityScale(0.0f);
		animations.SetAnimation("Die");
		die = true;
	}
	//yep, thats it

	animations.Update(position);

	desiredVelocity.y = 0.0f;

	if (!jumping)
	{
		if (run && !die)
		{
			desiredVelocity.x = RUNSPEED + runAddition;
		}
		else
			desiredVelocity.x = 0.0;
	}

	if(die)
		desiredVelocity.x = 0.0;
	
	if (oldPosition.x == position.x)
		standCounter++;
	else
		standCounter = 0;

		

	oldPosition = position;

	melee.Update(Entities);
	projectile.Update(Entities);
}

void Controllable::Draw(RenderWindow & Window)
{
	if (!(animations.GetFollower(0).animHandler->GetCurrentAnimation() == "Default" && die))
		animations.Draw(Window);
	else
		die = true;
	melee.Draw(Window);
}

void Controllable::ProcessInput(TypingControls &Controls)
{
	act = Controls.CheckWords(action);
	shoot = Controls.GetShoot();
}

void Controllable::Roll()
{
	run = false;
	roll = true;
	animations.SetAnimation("Slide");
	rollCounter = ROLLDURATION;
	b2Fixture*_fixture = body->GetFixtureList();
	while (_fixture->IsSensor())
		_fixture = _fixture->GetNext();
	body->DestroyFixture(_fixture);

	CreateRoundedFixture(Vector2f(ROLLSIZE, ROLLSIZE));
	body->GetFixtureList()->SetSensor(false);
}

void Controllable::Stand()
{
	roll = false;
	animations.SetAnimation("Default");
	b2Fixture*_fixture = body->GetFixtureList();
	while (_fixture->IsSensor())
		_fixture = _fixture->GetNext();
	body->DestroyFixture(_fixture);

	CreateRoundedFixture(Vector2f(ROLLSIZE, STANDSIZE));

	body->GetFixtureList()->SetSensor(false);
	run = true;
}

Controllable::Controllable(b2World &World)
{
	float _sensorHeight = 5.0f;
	action = "";

	CreateBody(World, Vector2f(ROLLSIZE, STANDSIZE));
	body->SetUserData((void*)ut::PLAYER);

	b2PolygonShape* _shape = new b2PolygonShape();
	_shape->SetAsBox(Utility::ScaleToB2((ROLLSIZE / 2) - ROUNDING), Utility::ScaleToB2(_sensorHeight), body->GetPosition() + Utility::SFVECtoB2VEC(Vector2f(0.0f, (STANDSIZE / 2) - (_sensorHeight / 2)), true), 0);

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = _shape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.1f;
	body->CreateFixture(&_fixtureDef);
	body->GetFixtureList()->SetSensor(true);

	_shape->SetAsBox(Utility::ScaleToB2(_sensorHeight), Utility::ScaleToB2(_sensorHeight), body->GetPosition() + Utility::SFVECtoB2VEC(Vector2f(ROLLSIZE / 2 + (_sensorHeight / 2), 0.0f), true), 0);
	_fixtureDef.shape = _shape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.1f;
	body->CreateFixture(&_fixtureDef);
	body->GetFixtureList()->SetSensor(true);

	projectile = Attack(this->body, 3.0f, Vector2f(ROLLSIZE / 2 + 10, -STANDSIZE / 2 - 10), Vector2f(10, -12), 0, 1, 0, 6, true, "Images/Arrow.png", Vector2i(1, 1));
	projectile.SetGravityScale(0.375f);
	melee = Attack(this->body, 7.5f, Vector2f(ROLLSIZE / 2 + 20, ROLLSIZE / 2 - 7.5), Vector2f(0, 0), 10, 3, 2, 0, false);

	desiredVelocity.x = RUNSPEED;
	run = true;

	vector<AnimationHandler::frameIndex>* _animations = new vector<AnimationHandler::frameIndex>();

	//----------BOTTOM ANIMATIONS----------//
	animations.LoadLeader("Images/Runner_Bottom.png", Vector2i(8, 3));
	_animations->push_back(AnimationHandler::frameIndex(0, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(1, Vector2f(0, 3)));
	_animations->push_back(AnimationHandler::frameIndex(2, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(3, Vector2f(0, 1)));
	_animations->push_back(AnimationHandler::frameIndex(4, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(5, Vector2f(0, 3)));
	_animations->push_back(AnimationHandler::frameIndex(6, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(7, Vector2f(0, 1)));
	animations.GetLeader().animHandler->CreateAnimation("Default", *_animations, true, false);
	animations.GetLeader().animHandler->SetCurrentAnimation("Default");
	animations.GetLeader().animHandler->SetDefaultAnimation("Default");
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(9, Vector2f(0, 1)));
	_animations->push_back(AnimationHandler::frameIndex(10, Vector2f(0, 4)));
	animations.GetLeader().animHandler->CreateAnimation("Slide", *_animations, false, true);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(8, Vector2f(0, 2)));
	animations.GetLeader().animHandler->CreateAnimation("Stand", *_animations, false, true);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(11, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(12, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(13, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(14, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(15, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(16, Vector2f(0, 2)));
	_animations->push_back(AnimationHandler::frameIndex(17, Vector2f(0, 2)));
	animations.GetLeader().animHandler->CreateAnimation("Die", *_animations, false, false);
	_animations->clear();
	//--------END BOTTOM ANIMATIONS--------//

	//-----------TOP ANIMATIONS-----------//
	animations.LoadFollower("Images/Runner_Top.png", Vector2i(8, 3), Vector2f(0, -7));
	_animations->push_back(AnimationHandler::frameIndex(0));
	_animations->push_back(AnimationHandler::frameIndex(1));
	_animations->push_back(AnimationHandler::frameIndex(2));
	_animations->push_back(AnimationHandler::frameIndex(3));
	_animations->push_back(AnimationHandler::frameIndex(4));
	_animations->push_back(AnimationHandler::frameIndex(5));
	_animations->push_back(AnimationHandler::frameIndex(6));
	_animations->push_back(AnimationHandler::frameIndex(7));
	animations.GetFollower(0).animHandler->CreateAnimation("Default", *_animations, true, false, true);
	animations.GetFollower(0).animHandler->SetCurrentAnimation("Default");
	animations.GetFollower(0).animHandler->SetDefaultAnimation("Default");
	animations.GetFollower(0).animHandler->SetFrameRate(3);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(9));
	_animations->push_back(AnimationHandler::frameIndex(10));
	_animations->push_back(AnimationHandler::frameIndex(9));
	_animations->push_back(AnimationHandler::frameIndex(8));
	_animations->push_back(AnimationHandler::frameIndex(9));
	_animations->push_back(AnimationHandler::frameIndex(10));
	animations.GetFollower(0).animHandler->CreateAnimation("Swipe", *_animations, false, false);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(11));
	animations.GetFollower(0).animHandler->CreateAnimation("Slide", *_animations, false, true);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(12));
	animations.GetFollower(0).animHandler->CreateAnimation("Stand", *_animations, false, true);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(13));
	animations.GetFollower(0).animHandler->CreateAnimation("LongPush", *_animations, false, false);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(14));
	animations.GetFollower(0).animHandler->CreateAnimation("ShortPush", *_animations, false, false);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(15));
	animations.GetFollower(0).animHandler->CreateAnimation("Shoot", *_animations, false, false);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(16));
	animations.GetFollower(0).animHandler->CreateAnimation("Release", *_animations, false, false);
	_animations->clear();

	_animations->push_back(AnimationHandler::frameIndex(17));
	_animations->push_back(AnimationHandler::frameIndex(18));
	_animations->push_back(AnimationHandler::frameIndex(19));
	_animations->push_back(AnimationHandler::frameIndex(20));
	_animations->push_back(AnimationHandler::frameIndex(21));
	_animations->push_back(AnimationHandler::frameIndex(22));
	_animations->push_back(AnimationHandler::frameIndex(23));
	animations.GetFollower(0).animHandler->CreateAnimation("Die", *_animations, false, false);
	_animations->clear();
	//---------END TOP ANIMATIONS---------//

	//----------ATTACK ANIMATIONS----------//
	vector<int>* _attackAnimations = new vector<int>();
	AnimationHandler _animation = AnimationHandler("Images/Swipe.png", Vector2i(9, 1));
	_animation.SetOrigin(Vector2f(16, 8));
	_attackAnimations->push_back(0);
	_attackAnimations->push_back(1);
	_attackAnimations->push_back(2);
	_attackAnimations->push_back(3);
	_attackAnimations->push_back(4);
	_attackAnimations->push_back(5);
	_attackAnimations->push_back(6);
	_attackAnimations->push_back(7);
	_attackAnimations->push_back(0);
	_attackAnimations->push_back(1);
	_attackAnimations->push_back(2);
	_attackAnimations->push_back(3);
	_attackAnimations->push_back(8);
	_animation.CreateAnimation("Swipe", *_attackAnimations, false, true);
	_animation.SetCurrentAnimation("Swipe");
	_animation.SetDefaultAnimation("Swipe");

	_animation.SetFrameRate(1);
	_animation.SetCurrentFrame(_attackAnimations->size() - 1);
	melee.SetAnimation(_animation);
	//--------END ATTACK ANIMATIONS--------//

	JumpBuffer.loadFromFile("Sounds/Jump1.ogg");
	JumpSound.setBuffer(JumpBuffer);
	RollBuffer.loadFromFile("Sounds/Zoop10.ogg");
	RollSound.setBuffer(RollBuffer);
	ShootBuffer.loadFromFile("Sounds/Zoop8.ogg");
	ShootSound.setBuffer(ShootBuffer);
	AttackBuffer.loadFromFile("Sounds/Flap4.ogg");
	AttackSound.setBuffer(AttackBuffer);
}


Controllable::Controllable()
{
}

Controllable::~Controllable()
{
}
