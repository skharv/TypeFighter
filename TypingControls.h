#ifndef TYPINGCONTROLS_H
#define TYPINGCONTROLS_H

#include "SFML\Graphics.hpp"
#include "DrawText.h"
#include "Utility.h"
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#define RADIUS 45

using namespace sf;
using namespace std;

class TypingControls
{
private:
	bool menu, options = false, go = false, drawSurrounds = true, shoot = false,  ok = false;
	int difficulty, difficultyOffset = 0, distanceCounter = 0, initialDistance = 0;
	string currentWord, distanceString;
	Texture texture, currentWordTexture, textPanel;
	Color textColour = Color::White;
	Sprite panelSprite;
	float radius;
	DrawText *drawable, *distanceText;

	struct Action
	{
		Action(string Name, Texture Texture) { name = Name; Drawable = new DrawText(Texture); };
		string name;
		vector<string> words;
		DrawText* Drawable;
	};

	vector<Action> actions;
	map<string, string> actionWords;
	vector<Vector2f> wordPositions;

	void PopulateWords();
	void PopulateInitialWords();
	void InitialiseWordPositions(float Radius);
public:
	void Update(Vector2f ScreenPosition, Vector2f ScreenSize);
	void Update(Vector2f ScreenPosition, Vector2f ScreenSize, Vector2f PlayerPosition, Vector2f PlayerVelocity, int Difficulty);
	void ProcessInput(Event &Event);
	void Draw(RenderWindow &Window, bool DrawWords);
	void DrawOK(RenderWindow &Window);
	void DrawPanel(RenderWindow &Window);
	bool CheckWords(string &ActionOutput);
	bool LoadWords(string Filepath);
	bool GetMenu() { return menu; };
	bool GetOK() { return ok; };
	bool GetOptions() { return options; };
	bool GetShoot() { return shoot; };
	int GetDistance() { return distanceCounter; };
	void SetMenu(bool Menu) { menu = Menu; };
	void SetOptions(bool Options) { options = Options; };
	void SetPlayerPosition(int PositionX) { initialDistance = PositionX; };

	TypingControls();
	TypingControls(int Difficulty, bool Menu);
	~TypingControls();
};


#endif