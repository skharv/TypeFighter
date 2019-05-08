#ifndef DISTANCEBOARD_H
#define DISTANCEBOARD_H

#include "LeaderboardHandler.h"
#include "DrawText.h"

#define LEADERBOARDSIZE 20

enum Alignment { Left, Center, Right };

class DistanceBoard
{
private:
	Vector2f position;
	int entries;
	float verticalGap;
	bool onTheLine = false;
	int alpha = 255;

	LeaderboardHandler *steamLeaderboard = NULL;
	DrawText *leaderboardHeading;
	DrawText *leaderboardEntry[LEADERBOARDSIZE];

	Alignment align;
public:
	Vector2f GetPosition() { return position; };
	void SetPosition(Vector2f Position) { position = Position; };
	void SetAlignment(Alignment Align) { align = Align; };
	void SetColourAlpha(int Alpha) { alpha = Alpha; };

	void Update();
	void Draw(RenderWindow &Window);

	void UploadDistance(int Distance);
	void UpdateBoard(Vector2f ScreenPosition);

	void ShutdownLeaderboard() { if (onTheLine) { SteamAPI_Shutdown(); } };

	DistanceBoard(const char* Name, Vector2f PositionOnScreen, Alignment Align, float VerticalGap);
	DistanceBoard();
	~DistanceBoard();
};

#endif