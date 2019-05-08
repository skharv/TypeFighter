#include "Engine.h"

bool Engine::Init()
{
	bool _init = true;

	srand(time(NULL));

	desktopSize = Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
	windowSize = Vector2f(WINDOWSIZEX, WINDOWSIZEY);

	window = new RenderWindow(VideoMode(desktopSize.x, desktopSize.y), "Keyboard Warrior"/*, Style::Fullscreen*/);
	window->setVerticalSyncEnabled(true);
	window->setKeyRepeatEnabled(false);
	window->setMouseCursorVisible(false);
	isFullscreen = true;

	volume = MAXVOLUME;

	music = new Music();
	music->openFromFile("Music/Menu.ogg");
	music->setLoop(true);
	music->setVolume(volume);
	music->play();

	world = new b2World(b2Vec2(0.0f, GRAVITY * ut::SCALE)); 
	camera = new Camera(Vector2f(windowSize.x / 4, windowSize.y / 4));
	camera->SetPosition(Vector2f(0, 120));

	SteamLeaderboards = new DistanceBoard("Distance Traveled", Vector2f(1, 1), Right, 1);
	SteamStatAchHandler = new StatsAchHandler();
	
	debugDraw = new SFMLDebugDraw(*window);
	//world->SetDebugDraw(debugDraw);
	uint32 _flags = 0;
	//_flags += b2Draw::e_aabbBit;
	//_flags += b2Draw::e_centerOfMassBit;
	//_flags += b2Draw::e_pairBit;
	//_flags += b2Draw::e_jointBit;
	_flags += b2Draw::e_shapeBit;
	debugDraw->SetFlags(_flags);

	collisionHandler = new CollisionHandler;
	world->SetContactListener(collisionHandler);

	distanceTravelled = new DrawText("Images/TestText.png", 8);

	menuShade = new RectangleShape(camera->GetCamera().getSize());
	menuShade->setFillColor(Color(0, 0, 0, 128));
	menuShade->setOrigin(Vector2f(menuShade->getSize().x / 2, menuShade->getSize().y / 2));
	menuShade->setPosition(camera->GetPosition());

	gameShade = new RectangleShape(camera->GetCamera().getSize());
	gameShade->setFillColor(Color::Black);
	gameShade->setOrigin(Vector2f(menuShade->getSize().x / 2, menuShade->getSize().y / 2));
	gameShade->setPosition(camera->GetPosition());
	shadeFader = 300;

 	entityManager = new EntityManager();
	background = new Background();
	
	int i = rand() % 3;

	switch (i)
	{
	case 0:
		background = new Background("Images/Skyline1.png", 1);
		break;
	case 1:
		background = new Background("Images/Skyline2.png", 2);
		break;
	case 2:
		background = new Background("Images/Skyline3.png", 3);
		break;
	default:
		break;
	}

	worldGenerator = new WorldGenerator("Images/Level1.png", *world);
	player = new Player(*world, isFullscreen);
	player->GetEntity()->SetPosition(worldGenerator->GetPlayerSpawn());
	entityManager->AddEntity(player->GetEntity());

	logo = new ImageHandler("Images/TypeFighter.png");
	logo->SetOrigin(Vector2f(logo->GetSize().x / 2, logo->GetSize().y / 2));
	logo->SetPosition(Vector2f(camera->GetPosition().x + (camera->GetSize().x / 2) - (logo->GetSize().x * 1.5), logo->GetSize().y));

	enemies = new EnemyGroup();
	destructables = new DestructableGroup();

	Resize();

	return _init;
}

void Engine::MainLoop()
{
	while (window->isOpen())
	{
		ProcessInput();

		Update();

		RenderFrame();
	}
}

void Engine::ProcessInput()
{
	Event _event;

	while (window->pollEvent(_event))
	{
		if (_event.type == Event::Closed)
			window->close();

		if ((Keyboard::isKeyPressed(Keyboard::F4) && (Keyboard::isKeyPressed(sf::Keyboard::LAlt) || Keyboard::isKeyPressed(Keyboard::RAlt))))
			window->close();

		if (player->IsAlive())
		{
			if(!player->GetEntity()->GetAct())
				player->ProcessInput(_event);
		}
		else
			player->ProcessInput(_event);


		if (_event.type == Event::Resized)
			Resize();
	}
}

void Engine::Update()
{
	world->Step(WORLDTIME, 8, 3);

	worldGenerator->Generate(*window, *world, camera->GetCamera().getCenter().x + (camera->GetCamera().getSize().x / 2) + 128);

	if (player->GetFullscreen() != isFullscreen)
	{
		if (isFullscreen)
			window->create(VideoMode(WINDOWSIZEX, WINDOWSIZEY), "Keyboard Warrior");
		else
			window->create(VideoMode(desktopSize.x, desktopSize.y), "Keyboard Warrior", Style::Fullscreen);

		window->setVerticalSyncEnabled(true);
		window->setKeyRepeatEnabled(false);
		window->setMouseCursorVisible(false);

		Resize();

		isFullscreen = !isFullscreen;
	}

	for (int i = 0; i < worldGenerator->GetEnemySpawns().size(); i++)
	{
		if (!worldGenerator->GetEnemySpawns()[i].spawned)
		{
			entityManager->AddEntity(enemies->Spawn(*world, worldGenerator->GetEnemySpawns()[i].position, worldGenerator->GetEnemySpawns()[i].classification));
			worldGenerator->EnemyHasSpawned(i);
		}
	}

	for (int i = 0; i < worldGenerator->GetDestructableSpawns().size(); i++)
	{
		if (!worldGenerator->GetDestructableSpawns()[i].spawned)
		{
			entityManager->AddEntity(destructables->Spawn(*world, worldGenerator->GetDestructableSpawns()[i].position, worldGenerator->GetDestructableSpawns()[i].classification));
			worldGenerator->DestructableHasSpawned(i);
		}
	}

	if(entityManager->GetEntities().size() > 0)
	{
		if (entityManager->GetEntities().at(0)->GetBody()->GetUserData() == ut::PLAYER && player->GetStandCounter() < 180)
		{
			camera->SetPosition(Vector2f(entityManager->GetEntities().at(0)->GetPosition().x + (camera->GetCamera().getSize().x / 4), camera->GetPosition().y));
		}
		else
		{
			if (player->IsAlive())
			{
				player->Kill();
				if(player->GetStandCounter() >= 180)
					SteamStatAchHandler->CompleteAchievement(ACH_STAND_STILL);
				else
					SteamStatAchHandler->CompleteAchievement(ACH_DIE);

				distanceTravelled->SetSprites(new string("YOU TRAVELLED: " + to_string(player->GetDistanceTravelled())));
				distanceTravelled->SetOrigin(Vector2f(distanceTravelled->GetSize().x / 2, distanceTravelled->GetSize().y / 2));
				distanceTravelled->SetPosition(Vector2f(camera->GetPosition().x - (camera->GetSize().x / 2) + (camera->GetSize().x / 20), camera->GetPosition().y - (camera->GetSize().y / 2.25)));

				SteamLeaderboards->UploadDistance(player->GetDistanceTravelled());
				SteamStatAchHandler->SetBestDistance(player->GetDistanceTravelled());

				if(player->GetDistanceTravelled() > 250)
					SteamStatAchHandler->CompleteAchievement(ACH_RUN_250);
				if (player->GetDistanceTravelled() > 500)
					SteamStatAchHandler->CompleteAchievement(ACH_RUN_500);
				if (player->GetDistanceTravelled() > 1000)
					SteamStatAchHandler->CompleteAchievement(ACH_RUN_1000);
				if (player->GetDistanceTravelled() > 10000)
					SteamStatAchHandler->CompleteAchievement(ACH_RUN_10000);

				shadeFader = 0;
			}
		}
	}
	else
	{
		if (player->IsAlive())
		{
			player->Kill();
			if (player->GetStandCounter() >= 180)
				SteamStatAchHandler->CompleteAchievement(ACH_STAND_STILL);
			else
				SteamStatAchHandler->CompleteAchievement(ACH_DIE);

			distanceTravelled->SetSprites(new string("YOU TRAVELLED: " + to_string(player->GetDistanceTravelled())));
			distanceTravelled->SetOrigin(Vector2f(distanceTravelled->GetSize().x / 2, distanceTravelled->GetSize().y / 2));
			distanceTravelled->SetPosition(Vector2f(camera->GetPosition().x - (camera->GetSize().x / 2) + (camera->GetSize().x / 20), camera->GetPosition().y - (camera->GetSize().y / 2.25)));

			SteamLeaderboards->UploadDistance(player->GetDistanceTravelled());
			SteamStatAchHandler->SetBestDistance(player->GetDistanceTravelled());

			if (player->GetDistanceTravelled() > 250)
				SteamStatAchHandler->CompleteAchievement(ACH_RUN_250);
			if (player->GetDistanceTravelled() > 500)
				SteamStatAchHandler->CompleteAchievement(ACH_RUN_500);
			if (player->GetDistanceTravelled() > 1000)
				SteamStatAchHandler->CompleteAchievement(ACH_RUN_1000);
			if (player->GetDistanceTravelled() > 10000)
				SteamStatAchHandler->CompleteAchievement(ACH_RUN_10000);

			shadeFader = 0;
		}
	}

	camera->Update(*window);

	if (!player->GetPlay())
	{
		menuShade->setPosition(camera->GetPosition());
		logo->SetPosition(Vector2f(camera->GetPosition().x + (camera->GetSize().x / 2) - (logo->GetSize().x / 1.5), (camera->GetPosition().y - (camera->GetSize().y / 2)) + (camera->GetPosition().y / 20) + logo->GetSize().y/1.5));
		SteamLeaderboards->UpdateBoard(Vector2f(camera->GetPosition().x + (camera->GetSize().x / 2) - (camera->GetSize().x / 20), camera->GetPosition().y - (camera->GetSize().y / 4)));
	}
	else
	{
		if (!gameOn)
		{
			music->openFromFile("Music/Game.ogg");
			music->play();
			gameOn = true;
			SteamStatAchHandler->CompleteAchievement(ACH_NAVIGATE_MENU);
		}
		worldGenerator->SetMenuGeneration(false);
		menuShade->setFillColor(sf::Color::Transparent);
	}

	if (!player->IsAlive())
	{
		if (shadeFader < 300)
			shadeFader++;
		if(volume >= 0)
			music->setVolume(max(--volume, 0));
		
		SteamLeaderboards->UpdateBoard(Vector2f(camera->GetPosition().x + (camera->GetSize().x / 2) - (camera->GetSize().x / 20), camera->GetPosition().y - (camera->GetSize().y / 2.25)));
	}
	else
	{
		if (shadeFader != 0)
		{
			shadeFader--;
			if (volume <= MAXVOLUME)
				music->setVolume(min(++volume, MAXVOLUME));
		}
	}

	gameShade->setPosition(camera->GetPosition());
	gameShade->setFillColor(sf::Color(0, 0, 0, min(shadeFader, 255)));
	distanceTravelled->SetColour(sf::Color(255, 255, 255, min(shadeFader, 255)));

	if (!player->GetPlay() && player->IsAlive())
		SteamLeaderboards->SetColourAlpha(255);
	else
		SteamLeaderboards->SetColourAlpha(min(shadeFader, 255));
	
	player->Update(camera->GetPosition(), camera->GetCamera().getSize());

	if (collisionHandler->GetJustKilled())
	{
		if (collisionHandler->GetLastDeathType() == BULLET)
			SteamStatAchHandler->CompleteAchievement(ACH_SHOOT_SOMETHING);
		if (collisionHandler->GetLastDeathType() == MELEE)
			SteamStatAchHandler->CompleteAchievement(ACH_SLASH_SOMETHING);
		collisionHandler->ResetJustKilled();
	}

	if (player->GetWordCounter() > 0)
	{
		SteamStatAchHandler->IncrementWordStat();
		player->ResetWordCounter();
	}

	if (shadeFader >= 300)
	{
		if(player->GetOK())
			Restart();
	}
	else
	{
		entityManager->Update(camera->GetDespawnPoint());

		if (player->IsAlive())
			background->Update(camera->GetPosition(), camera->GetSize(), player->GetEntity()->GetVelocity());

		worldGenerator->Update();
		worldGenerator->Degenerate(camera->GetDespawnPoint());

		vector<b2Body*> _hitList;
		entityManager->PopulateDead(_hitList);
		worldGenerator->PopulateDead(_hitList);

		for (int i = 0; i < _hitList.size(); i++)
		{
			world->DestroyBody(_hitList[i]);
		}
	}

	SteamLeaderboards->Update();

	if (player->GetQuit())
	{
		SteamLeaderboards->ShutdownLeaderboard();
		window->close();
	}
}

void Engine::RenderFrame()
{
	window->clear(sf::Color::Black);
	background->Draw(*window);
	worldGenerator->Draw(*window);
	entityManager->Draw(*window);
	window->draw(*menuShade);
	if (!player->GetPlay() && player->IsAlive())
	{
		SteamLeaderboards->Draw(*window);
		logo->Draw(*window);
	}
	player->Draw(*window);
	window->draw(*gameShade);

	if (!player->IsAlive())
	{
		distanceTravelled->Draw(*window);
		SteamLeaderboards->Draw(*window);
		player->DrawPanel(*window);
	}
	if (shadeFader >= 300)
	{
		player->DrawOK(*window);
	}

	world->DrawDebugData();

	window->display();
}

void Engine::Restart()
{
	vector<b2Body*> _hitList;
	entityManager->KillAll(_hitList);
	for (int i = 0; i < _hitList.size(); i++)
	{
		world->DestroyBody(_hitList[i]);
	}

	worldGenerator->ClearWorld();
	worldGenerator->SetMenuGeneration(true);

	gameOn = false;
	music->openFromFile("Music/Menu.ogg");
	music->play();

	int i = rand() % 3;

	switch (i)
	{
	case 0:
		background = new Background("Images/Skyline1.png", camera->GetPosition(), 1);
		break;
	case 1:
		background = new Background("Images/Skyline2.png", camera->GetPosition(), 2);
		break;
	case 2:
		background = new Background("Images/Skyline3.png", camera->GetPosition(), 3);
		break;
	default:
		break;
	}

	camera->SetPosition(Vector2f(0, camera->GetPosition().y));

	menuShade->setFillColor(sf::Color(0, 0, 0, 128));
	menuShade->setPosition(camera->GetPosition());

	worldGenerator->GenerateStart(*world);

	player = new Player(*world, isFullscreen);
	player->GetEntity()->SetPosition(worldGenerator->GetPlayerSpawn());
	entityManager->AddEntity(player->GetEntity());

	shadeFader = 300;
}

void Engine::Resize()
{
	int minX = 480;
	int minY = 270;

	if (window->getSize().x < minX)
		window->setSize(Vector2u(minX, window->getSize().y));
	if (window->getSize().y < minY)
		window->setSize(Vector2u(window->getSize().x, minY));

	int zoomX = (float)window->getSize().x / minX;
	float zoomXPercent = 1 / (float)zoomX;

	int zoomY = (float)window->getSize().y / minY;
	float zoomYPercent = 1 / (float)zoomY;

	camera->Resize(window->getSize());

	zoomXPercent < zoomYPercent ? camera->SetZoom(zoomYPercent) : camera->SetZoom(zoomXPercent);

	menuShade->setSize(Vector2f(window->getSize()));
	menuShade->setOrigin(Vector2f(menuShade->getSize().x / 2, menuShade->getSize().y / 2));
	gameShade->setSize(Vector2f(window->getSize()));
	gameShade->setOrigin(Vector2f(menuShade->getSize().x / 2, menuShade->getSize().y / 2));
}

void Engine::Go()
{
	if (!Init())
		throw "Could not initialise Engine";

	MainLoop();
}

Engine::Engine()
{

}


Engine::~Engine()
{
}
