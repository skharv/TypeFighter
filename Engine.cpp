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

	
	bool bRet = SteamAPI_Init();

	leaderboardHeading = new DrawText("Images/TestText.png", 8);
	leaderboardHeading->SetSprites(new string("LEADERBOARD:"));

	if (bRet)
	{
		g_SteamLeaderboards = new Leaderboard();
		g_SteamLeaderboards->FindLeaderboard("Distance Traveled");

		for (int i = 0; i < LEADERBOARDSIZE; i++)
		{
			leaderboardDistance[i] = new DrawText("Images/TestText.png", 8);

			string s;
			s = "NULL";

			leaderboardDistance[i]->SetSprites(&s);
		}
	}
	
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

	SteamAPI_RunCallbacks();

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

				distanceTravelled->SetSprites(new string("YOU TRAVELLED: " + to_string(player->GetDistanceTravelled())));
				distanceTravelled->SetOrigin(Vector2f((distanceTravelled->GetLength().x) / 2, distanceTravelled->GetLength().y / 2));
				distanceTravelled->SetPosition(Vector2f(camera->GetPosition().x, camera->GetPosition().y));

				//g_SteamLeaderboards->FindLeaderboard("Distance Traveled");
				g_SteamLeaderboards->UploadScore(player->GetDistanceTravelled());

				leaderboardHeading->SetOrigin(Vector2f((leaderboardHeading->GetLength().x) / 2, leaderboardHeading->GetLength().y / 2));
				leaderboardHeading->SetPosition(Vector2f(camera->GetStartPoint().x + leaderboardHeading->GetLength().x / 2, camera->GetStartPoint().y + leaderboardHeading->GetLength().y / 2));
				
				g_SteamLeaderboards->DownloadScore();

				shadeFader = 0;
			}
		}
	}
	else
	{
		if (player->IsAlive())
		{
			player->Kill();

			distanceTravelled->SetSprites(new string("YOU TRAVELLED: " + to_string(player->GetDistanceTravelled())));
			distanceTravelled->SetOrigin(Vector2f((distanceTravelled->GetLength().x) / 2, distanceTravelled->GetLength().y / 2));
			distanceTravelled->SetPosition(Vector2f(camera->GetPosition().x, camera->GetPosition().y));

			//g_SteamLeaderboards->FindLeaderboard("Distance Traveled");
			g_SteamLeaderboards->UploadScore(player->GetDistanceTravelled());

			leaderboardHeading->SetOrigin(Vector2f((leaderboardHeading->GetLength().x) / 2, leaderboardHeading->GetLength().y / 2));
			leaderboardHeading->SetPosition(Vector2f(camera->GetStartPoint().x + leaderboardHeading->GetLength().x / 2, camera->GetStartPoint().y + leaderboardHeading->GetLength().y / 2));

			g_SteamLeaderboards->DownloadScore();

			shadeFader = 0;
		}
	}

	camera->Update(*window);

	if (!player->GetPlay())
	{
		menuShade->setPosition(camera->GetPosition());
		logo->SetPosition(Vector2f(camera->GetPosition().x + (camera->GetSize().x / 2) - (logo->GetSize().x / 1.5), (camera->GetPosition().y - (camera->GetSize().y / 2)) + (camera->GetPosition().y / 20) + logo->GetSize().y/1.5));
	}
	else
	{
		if (!gameOn)
		{
			music->openFromFile("Music/Game.ogg");
			music->play();
			gameOn = true;
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

		for (int i = 0; i < LEADERBOARDSIZE; i++)
		{
			leaderboardDistance[i]->SetPosition(Vector2f(leaderboardHeading->GetPosition().x, leaderboardHeading->GetPosition().y + (leaderboardDistance[i]->GetSize().y * (i + 1)) + (leaderboardDistance[i]->GetLength().y / 2)));
			leaderboardDistance[i]->SetOrigin(Vector2f((leaderboardDistance[i]->GetLength().x) / 2, (leaderboardDistance[i]->GetLength().y / 2)));

			string s;
			s = to_string(g_SteamLeaderboards->leaderboardEntries[i].m_nGlobalRank);
			s += " ";
			s += to_string(g_SteamLeaderboards->leaderboardEntries[i].m_steamIDUser.GetAccountID());
			s += " ";
			s += to_string(g_SteamLeaderboards->leaderboardEntries[i].m_nScore);

			leaderboardDistance[i]->SetSprites(&s);
		}
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
	leaderboardHeading->SetColour(sf::Color(255, 255, 255, min(shadeFader, 255)));
	for (int i = 0; i < LEADERBOARDSIZE; i++)
		leaderboardDistance[i]->SetColour(sf::Color(255, 255, 255, min(shadeFader, 255)));

	player->Update(camera->GetPosition(), camera->GetCamera().getSize());

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

	if (player->GetQuit())
	{
		SteamAPI_Shutdown();
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
		logo->Draw(*window);
	player->Draw(*window);
	window->draw(*gameShade);

	if (!player->IsAlive())
	{
		distanceTravelled->Draw(*window);
		leaderboardHeading->Draw(*window);
		for (int i = 0; i < LEADERBOARDSIZE; i++)
			leaderboardDistance[i]->Draw(*window);
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

	camera->SetPosition(Vector2f(0, camera->GetPosition().y));

	menuShade->setFillColor(sf::Color(0, 0, 0, 128));
	menuShade->setPosition(camera->GetPosition());

	worldGenerator->GenerateStart(*world);

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

	player = new Player(*world, isFullscreen);
	player->GetEntity()->SetPosition(worldGenerator->GetPlayerSpawn());
	entityManager->AddEntity(player->GetEntity());

	shadeFader = 300;
}

void Engine::Resize()
{
	if (window->getSize().x < 640)
		window->setSize(Vector2u(640, window->getSize().y));
	if (window->getSize().y < 640)
		window->setSize(Vector2u(window->getSize().x, 640));

	camera->Resize(window->getSize());
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
