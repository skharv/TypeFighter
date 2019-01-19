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

			SpawnObjects(_spawn, Vector2f(CameraPosition + CameraSize));
		}

		for (int i = 0; i < mountains.size(); i++)
		{
			if (mountains[i]->GetPosition().x < (CameraPosition.x - CameraSize.x))
			{
				mountains[i]->~ImageHandler();
				mountains.erase(mountains.begin() + i);
				continue;
			}

			mountains[i]->Update(Vector2f(mountains[i]->GetPosition().x + (PlayerSpeed.x / MOUNTAINSPEED), mountains[i]->GetPosition().y));
		}

	background->Update(CameraPosition);
}

void Background::Draw(RenderWindow & Window)
{
	background->Draw(Window);

	for (int i = 0; i < mountains.size(); i++)
	{
		mountains[i]->Draw(Window);
	}
}

void Background::SpawnObjects(string FilePath, Vector2f Position)
{
	ImageHandler *_mountain = new ImageHandler(FilePath);
	int _spawnHeight = (rand() % MOUNTAINSPAWNVARIANT) + MOUNTAINSPAWNHEIGHT - (_mountain->GetSize().y/2);

	_mountain->SetPosition(Vector2f(Position.x, _spawnHeight));

	mountains.push_back(_mountain);
}

void Background::SpawnObjects(Vector2f Position)
{
	int _spawnNumber = rand() % 3;
	ImageHandler *_mountain = new ImageHandler(mountainTextures[_spawnNumber]);
	int _spawnHeight = (rand() % MOUNTAINSPAWNVARIANT) + MOUNTAINSPAWNHEIGHT - (_mountain->GetSize().y / 2);
	_mountain->SetPosition(Vector2f(Position.x, _spawnHeight));
	mountains.push_back(_mountain);
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

	SpawnObjects();
}

Background::Background()
{

}


Background::~Background()
{
}
