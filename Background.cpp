#include "Background.h"

void Background::Update(Vector2f CameraPosition, Vector2f CameraSize, Vector2f PlayerSpeed)
{
	if ((mountains.back()->GetPosition().x + spawnPosition) < (CameraPosition.x + CameraSize.x))
	{
		if (mountains.size() > 0)
			spawnPosition = rand() % (mountains[0]->GetSize().x);

		string _spawn = MOUNTAINSTRING;
		_spawn.append(to_string(backgroundID));
		int _spawnNumber = rand() % 3 + 1;
		_spawn.append(to_string(_spawnNumber));
		_spawn.append(".png");

		SpawnMountain(_spawn, Vector2f(CameraPosition + CameraSize));
	}

	if ((clouds.back()->GetPosition().x + spawnPosition) < (CameraPosition.x + CameraSize.x))
	{
		int j = rand() % cloudDensity + 1;
		for (int i = 0; i < j; i++)
		{
			if (clouds.size() > 0)
				spawnPosition = rand() % (clouds[0]->GetSize().x);

			string _spawn = CLOUDSTRING;
			int _spawnNumber = rand() % 12;
			_spawn.append(to_string(_spawnNumber));
			_spawn.append(".png");

			SpawnCloud(_spawn, Vector2f(CameraPosition + CameraSize));
		}
	}

	for (int i = 0; i < mountains.size(); i++)
	{
		if (mountains[i]->GetPosition().x < (CameraPosition.x - CameraSize.x))
		{
			mountains[i]->~ImageHandler();
			mountains.erase(mountains.begin() + i);
			continue;
		}

		mountains[i]->SetPositionOffset(Vector2f((MOUNTAINSPEED * PlayerSpeed.x) / 100, 0));
		mountains[i]->Update(mountains[i]->GetPosition());
	}

	for (int i = 0; i < clouds.size(); i++)
	{
		if (clouds[i]->GetPosition().x < (CameraPosition.x - CameraSize.x))
		{
			clouds[i]->~ImageHandler();
			clouds.erase(clouds.begin() + i);
			continue;
		}

		clouds[i]->SetPositionOffset(Vector2f((CLOUDSPEEDMULTIPLIER * PlayerSpeed.x) / 100, 0));
		clouds[i]->Update(clouds[i]->GetPosition());
	}

	background->Update(CameraPosition);
}

void Background::Draw(RenderWindow & Window)
{
	background->Draw(Window);

	for (int i = 0; i < clouds.size(); i++)
	{
		clouds[i]->Draw(Window);
	}
	for (int i = 0; i < mountains.size(); i++)
	{
		mountains[i]->Draw(Window);
	}
}

void Background::SpawnMountain(string FilePath, Vector2f Position)
{
	ImageHandler *_mountain = new ImageHandler(FilePath);
	int _spawnHeight = (rand() % MOUNTAINSPAWNVARIANT) + MOUNTAINSPAWNHEIGHT - (_mountain->GetSize().y/2);

	_mountain->SetPosition(Vector2f(Position.x, _spawnHeight));

	mountains.push_back(_mountain);
}

void Background::SpawnMountain(Vector2f Position)
{
	int _spawnNumber = rand() % 3;
	ImageHandler *_mountain = new ImageHandler(mountainTextures[_spawnNumber]);
	int _spawnHeight = (rand() % MOUNTAINSPAWNVARIANT) + MOUNTAINSPAWNHEIGHT - (_mountain->GetSize().y / 2);
	_mountain->SetPosition(Vector2f(Position.x, _spawnHeight));
	mountains.push_back(_mountain);
}

void Background::SpawnCloud(string FilePath, Vector2f Position)
{
	ImageHandler *_cloud = new ImageHandler(FilePath);
	int _spawnHeight = (rand() % CLOUDSPAWNVARIANT) + CLOUDSPAWNHEIGHT - (_cloud->GetSize().y / 2);
	int _spawnWidth = (rand() % CLOUDSPAWNVARIANT);

	_cloud->SetPosition(Vector2f(Position.x + _spawnWidth, _spawnHeight));

	clouds.push_back(_cloud);
}

void Background::SpawnCloud(Vector2f Position)
{
	int _spawnNumber = rand() % 12;
	ImageHandler *_cloud = new ImageHandler(cloudTextures[_spawnNumber]);
	int _spawnHeight = (rand() % CLOUDSPAWNVARIANT) + CLOUDSPAWNHEIGHT - (_cloud->GetSize().y / 2);
	int _spawnWidth = (rand() % CLOUDSPAWNVARIANT);

	_cloud->SetPosition(Vector2f(Position.x + _spawnWidth, _spawnHeight));

	clouds.push_back(_cloud);
}

Background::Background(string FilePath, int Id)
{
	background = new ImageHandler(FilePath);
	backgroundID = Id;

	Texture _texture;
	for (int i = 1; i < 4; i++)
	{
		string _textureString = MOUNTAINSTRING;
		_textureString.append(to_string(backgroundID));
		_textureString.append(to_string(i));
		_textureString.append(".png");
		_texture.loadFromFile(_textureString);
		mountainTextures.push_back(_texture);
	}

	for (int i = 0; i < 12; i++)
	{
		string _textureString = CLOUDSTRING;
		_textureString.append(to_string(i));
		_textureString.append(".png");
		_texture.loadFromFile(_textureString);
		cloudTextures.push_back(_texture);
	}

	switch (rand() % 3)
	{
	case 0:
		cloudDensity = 3;
		break;
	case 1:
		cloudDensity = 6;
		break;
	case 2:
		cloudDensity = 9;
		break;
	default:
		cloudDensity = 12;
		break;
	}

	for (int i = 0; i < 5; i++)
	{
		SpawnMountain(Vector2f(i * 150, 0));
		for (int j = 0; j < cloudDensity; j++)
			SpawnCloud(Vector2f(i * 150, 0));
	}
}

Background::Background(string FilePath, Vector2f ScreenPosition, int Id)
{
	background = new ImageHandler(FilePath);
	backgroundID = Id;

	Texture _texture;
	for (int i = 1; i < 4; i++)
	{
		string _textureString = MOUNTAINSTRING;
		_textureString.append(to_string(backgroundID));
		_textureString.append(to_string(i));
		_textureString.append(".png");
		_texture.loadFromFile(_textureString);
		mountainTextures.push_back(_texture);
	}

	for (int i = 0; i < 12; i++)
	{
		string _textureString = CLOUDSTRING;
		_textureString.append(to_string(i));
		_textureString.append(".png");
		_texture.loadFromFile(_textureString);
		cloudTextures.push_back(_texture);
	}

	switch (rand() % 3)
	{
	case 0:
		cloudDensity = 3;
		break;
	case 1:
		cloudDensity = 6;
		break;
	case 2:
		cloudDensity = 9;
		break;
	default:
		cloudDensity = 12;
		break;
	}

	for (int i = 0; i < 5; i++)
	{
		SpawnMountain(Vector2f(ScreenPosition.x + ((i + 3) * 150), 0));
		for (int j = 0; j < cloudDensity; j++)
			SpawnCloud(Vector2f(ScreenPosition.x + ((i + 3) * 150), 0));
	}
}

Background::Background()
{

}


Background::~Background()
{
}
