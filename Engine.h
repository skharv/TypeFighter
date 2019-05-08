#ifndef ENGINE_H
#define ENGINE_H

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <Box2D\Box2D.h>
#include <steam\steam_api.h>
#include "WorldGenerator.h"
#include "ImageHandler.h"
#include "Background.h"
#include "EnemyGroup.h"
#include "DestructableGroup.h"
#include "EntityManager.h"
#include "Controllable.h"
#include "Camera.h"
#include "SFMLDebugDraw.h"
#include "Utility.h"
#include "Player.h"
#include "CollisionHandler.h"
#include "DistanceBoard.h"
#include "StatsAchHandler.h"

#define GRAVITY 2.5f
#define WORLDTIME 1/60.0f
#define WINDOWSIZEX 1600
#define WINDOWSIZEY 900
#define MAXVOLUME 100
#define VERSION 2.0f

using namespace std;
using namespace sf;

class Engine
{
private:
	RenderWindow *window;
	Vector2f desktopSize, windowSize;
	Music *music;
	 
	WorldGenerator *worldGenerator;
	Background *background;
	DrawText *distanceTravelled;
	DrawText *leaderboardHeading;
	DrawText *leaderboardDistance[LEADERBOARDSIZE];

	DistanceBoard *SteamLeaderboards = NULL;
	StatsAchHandler *SteamStatAchHandler = NULL;

	Camera *camera;
	b2World *world;
	CollisionHandler *collisionHandler;
	SFMLDebugDraw *debugDraw;

	EntityManager *entityManager;
	Player *player;

	EnemyGroup *enemies;
	DestructableGroup *destructables;

	ImageHandler *logo;
	RectangleShape *menuShade, *gameShade;
	int shadeFader, volume;
	bool isFullscreen = false;
	bool gameOn = false;

	bool Init();
	void MainLoop();
	void ProcessInput();
	void Update();
	void RenderFrame();

	void Restart();
	void Resize();
public:
	void Go();
	Engine();
	~Engine();
};

#endif // !ENGINE_H


