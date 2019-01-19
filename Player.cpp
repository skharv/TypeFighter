#include "Player.h"

void Player::Draw(RenderWindow &Window)
{
	controls->Draw(Window, play);
}

void Player::ProcessInput(Event Event)
{
	if (isAlive)
	{
		controls->ProcessInput(Event);
		if (play)
			entity->ProcessInput(*controls);
	}
	else
	{
		controls->ProcessInput(Event);
	}
}

void Player::Update(Vector2f CameraPosition, Vector2f CameraSize)
{
	if (isAlive)
		controls->Update(CameraPosition, CameraSize, entity->GetPosition(), Utility::B2VECtoSFVEC(entity->GetBody()->GetLinearVelocity(), true), MINIMUMDIFFICULTY);
	else
	{
		string _okString = "";
		controls->Update(CameraPosition, CameraSize);

		if (controls->CheckWords(_okString))
			if (_okString == "OK")
				ok = true;
	}

	if (controls->GetMenu())
	{
		string _menu = "";

		if (controls->CheckWords(_menu))
		{
			if (!controls->GetOptions())
			{
				if (_menu == "START")
				{
					StartGame();
					controls->SetPlayerPosition(entity->GetPosition().x);
				}
				if (_menu == "OPTIONS")
					controls->SetOptions(true);
				if (_menu == "QUIT")
					quit = true;
			}
			else
			{
				if (_menu == "BACK")
					controls->SetOptions(false);
				if (_menu == "FULLSCREEN")
				{
					ToggleFullscreen();
				}
			}
		}
	}

}

Player::Player(b2World &World, bool Fullscreen)
{
	entity = new Controllable(World);
	controls = new TypingControls(MINIMUMDIFFICULTY, true);
	fullscreen = Fullscreen;
}

Player::Player()
{
}


Player::~Player()
{
}
