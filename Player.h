#ifndef PLAYER_H
#define PLAYER_H

#include <SFML\Graphics.hpp>
#include "Controllable.h"
#include "TypingControls.h"

#define MINIMUMDIFFICULTY 5

class Player
{
private:
	bool isAlive = true;
	bool play = false;
	bool quit = false;
	bool ok = false;
	bool fullscreen;
	Controllable *entity;
	TypingControls *controls;
	int difficulty;

public:
	void Draw(RenderWindow &Window);
	void DrawOK(RenderWindow &Window) { controls->DrawOK(Window); };
	void DrawPanel(RenderWindow &Window) { controls->DrawPanel(Window); };
	void ProcessInput(Event Event);
	void Update(Vector2f CameraPosition, Vector2f CameraSize);
	void StartGame() { play = true; controls->SetMenu(false); };
	bool IsAlive() { return isAlive; };
	bool GetPlay() { return play; };
	bool GetOK() { return ok; };
	bool GetQuit() { return quit; };
	bool GetFullscreen() { return fullscreen; };
	void ToggleFullscreen() { fullscreen = !fullscreen; };
	int GetDistanceTravelled() { return controls->GetDistance(); };
	int GetStandCounter() { return entity->GetStandCounter(); };

	void Kill() { isAlive = false; play = false;};
	Controllable *GetEntity() { return entity; };

	Player(b2World &World, bool Fullscreen = false);
	Player();
	~Player();
};

#endif