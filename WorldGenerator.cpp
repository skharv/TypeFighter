#include "WorldGenerator.h"

bool WorldGenerator::CreateSectionsFromImage(string Filepath)
{
	bool _return = true;

	if (!image.loadFromFile(Filepath))
	{
		_return = false;
	}
	else
	{
		sectionCount = image.getSize().x / SECTIONWIDTH;

		for (int z = 0; z < sectionCount; z++)
		{
			WorldSection* _section = new WorldSection();
			for (int x = 0; x < SECTIONWIDTH; x++)
			{
				for (int y = 0; y < image.getSize().y; y++)
				{
					Color _colour = Color(image.getPixel(x + (z * SECTIONWIDTH), y).r, image.getPixel(x + (z * SECTIONWIDTH), y).g, image.getPixel(x + (z * SECTIONWIDTH), y).b, image.getPixel(x + (z * SECTIONWIDTH), y).a);

					TileData* _tileData = new TileData(_colour, Vector2f(x, y), Vector2f(TILESIZE, TILESIZE));

					if (x > 0 && _section->tiles.size() >= image.getSize().y)
					{
						_tileData->left = new TileData(_section->tiles.at(_section->tiles.size() - image.getSize().y));
						_section->tiles.at(_section->tiles.size() - image.getSize().y).right = _tileData;
					}
					else if (x == 0)
						_tileData->left = _tileData;
					if (x == SECTIONWIDTH - 1)
						_tileData->right = _tileData;

					if (y > 0 && _section->tiles.size() > 0)
					{
						_tileData->up = new TileData(_section->tiles.back());
						_section->tiles.back().down = _tileData;
					}
					else if (y == 0)
						_tileData->up = _tileData;
					if (y == image.getSize().y - 1)
						_tileData->down = _tileData;

					Vector2i _sections = Vector2i(texture.getSize().x / TILEIMAGESIZE, texture.getSize().y / TILEIMAGESIZE);

					_tileData->tileIndex = rand() % 6;
					if (_tileData->colour == Color(128, 128, 128, 255))
						_tileData->tileIndex = 6;
					if (_tileData->colour == Color::Black)
						_tileData->tileIndex = 7;

					_tileData->sprite = (Sprite(texture, IntRect((_tileData->tileIndex % _sections.x) * TILEIMAGESIZE, (_tileData->tileIndex / _sections.x) * TILEIMAGESIZE, TILEIMAGESIZE, TILEIMAGESIZE)));

					_section->tiles.push_back(*_tileData);
				}
			}

			worldSections.push_back(*_section);
		}
	}

	return _return;
}

void WorldGenerator::SpawnSection(int Index, b2World &World)
{
	b2Body* _body;

	vector<b2FixtureDef*> _fixtureDefs;

	Vector2f _halfSize = Vector2f((TILESIZE / 2), (TILESIZE / 2));

	Vector2f _position = Vector2f(worldSections[Index].tiles[0].position.x * TILESIZE + (spawnCount * TILESIZE * SECTIONWIDTH), worldSections[Index].tiles[0].position.y * TILESIZE);

	IntRect _backSectionRect;
	SpawnedSection _newSection;
	_newSection.position = _position;
	_newSection.backgroundSprite = new Sprite();
	_newSection.backgroundSprite->setPosition(_position);
	_newSection.backgroundSprite->setOrigin(_halfSize);
	_newSection.backgroundSprite->setTexture(backSectionTexture);
	if(Index == 5)
		_backSectionRect = IntRect(backSectionTexture.getSize().x / 2, 0, backSectionTexture.getSize().x / 2, backSectionTexture.getSize().y);
	else
		_backSectionRect = IntRect(0, 0, backSectionTexture.getSize().x / 2, backSectionTexture.getSize().y);
	_newSection.backgroundSprite->setTextureRect(_backSectionRect);
	_newSection.worldSection = worldSections[Index];
	spawnedSections.push_back(_newSection);

	b2BodyDef _bodyDef;
	_bodyDef.type = b2_staticBody;
	_bodyDef.position.Set(Utility::ScaleToB2(_position.x), Utility::ScaleToB2(_position.y));
	_body = World.CreateBody(&_bodyDef);

	b2FixtureDef* _fixtureDef;
	_fixtureDef = new b2FixtureDef();	

	for (int x = 0; x < worldSections[Index].tiles.size(); x++)
	{
		if (worldSections[Index].tiles[x].colour == Color::White)
		{
			if (worldSections[Index].tiles[x].up->colour != Color::White)
			{
				Vector2f _startPosition, _endPosition;

				_startPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) - (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) - (TILESIZE / 2));
				_endPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) + (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) - (TILESIZE / 2));

				b2EdgeShape* _shape = new b2EdgeShape();
				_shape->Set(Utility::SFVECtoB2VEC(_startPosition, true), Utility::SFVECtoB2VEC(_endPosition, true));

				_fixtureDef = new b2FixtureDef();
				_fixtureDef->shape = _shape;
				_fixtureDef->density = DENSITY;
				_fixtureDef->friction = FRICTION;

				_body->CreateFixture(_fixtureDef);
			}
			if ((worldSections[Index].tiles[x].right->colour != Color::White) || (worldSections[Index].tiles[x].position.x == SECTIONWIDTH - 1))
			{
				Vector2f _startPosition, _endPosition;

				_startPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) + (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) - (TILESIZE / 2));
				_endPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) + (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) + (TILESIZE / 2));

				b2EdgeShape* _shape = new b2EdgeShape();
				_shape->Set(Utility::SFVECtoB2VEC(_startPosition, true), Utility::SFVECtoB2VEC(_endPosition, true));

				_fixtureDef = new b2FixtureDef();
				_fixtureDef->shape = _shape;
				_fixtureDef->density = DENSITY;
				_fixtureDef->friction = FRICTION;

				_body->CreateFixture(_fixtureDef);
			}
			if (worldSections[Index].tiles[x].down->colour != Color::White)
			{
				Vector2f _startPosition, _endPosition;

				_startPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) + (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) + (TILESIZE / 2));
				_endPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) - (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) + (TILESIZE / 2));

				b2EdgeShape* _shape = new b2EdgeShape();
				_shape->Set(Utility::SFVECtoB2VEC(_startPosition, true), Utility::SFVECtoB2VEC(_endPosition, true));

				_fixtureDef = new b2FixtureDef();
				_fixtureDef->shape = _shape;
				_fixtureDef->density = DENSITY;
				_fixtureDef->friction = FRICTION;

				_body->CreateFixture(_fixtureDef);
			}
			if ((worldSections[Index].tiles[x].left->colour != Color::White) || (worldSections[Index].tiles[x].position.x == 0))
			{
				Vector2f _startPosition, _endPosition;

				_startPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) - (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) + (TILESIZE / 2));
				_endPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) - (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) - (TILESIZE / 2));

				b2EdgeShape* _shape = new b2EdgeShape();
				_shape->Set(Utility::SFVECtoB2VEC(_startPosition, true), Utility::SFVECtoB2VEC(_endPosition, true));

				_fixtureDef = new b2FixtureDef();
				_fixtureDef->shape = _shape;
				_fixtureDef->density = DENSITY;
				_fixtureDef->friction = FRICTION;

				_body->CreateFixture(_fixtureDef);
			}
		}
		if (worldSections[Index].tiles[x].colour == Color::Blue)
		{
			Vector2f _destructablePosition = _position + Vector2f(worldSections[Index].tiles[x].position.x * TILESIZE, worldSections[Index].tiles[x].position.y * TILESIZE);
			int _classification = 0;
			destructableSpawns.push_back(DestructableSpawn(_destructablePosition, _classification));
		}
		if (worldSections[Index].tiles[x].colour == Color::Cyan)
		{
			Vector2f _destructablePosition = _position + Vector2f(worldSections[Index].tiles[x].position.x * TILESIZE, worldSections[Index].tiles[x].position.y * TILESIZE);
			int _classification = 1;
			destructableSpawns.push_back(DestructableSpawn(_destructablePosition, _classification));
		}
		if (worldSections[Index].tiles[x].colour == Color::Green)
		{
			Vector2f _spawnPosition = _position + Vector2f(worldSections[Index].tiles[x].position.x * TILESIZE, worldSections[Index].tiles[x].position.y * TILESIZE);
			playerSpawn = _spawnPosition;
		}
		if (worldSections[Index].tiles[x].colour == Color::Red)
		{
			Vector2f _enemyPosition = _position + Vector2f(worldSections[Index].tiles[x].position.x * TILESIZE, worldSections[Index].tiles[x].position.y * TILESIZE);
			int _classification = 0;
			enemySpawns.push_back(EnemySpawn(_enemyPosition, _classification));
		}
		if (worldSections[Index].tiles[x].colour == Color::Yellow)
		{
			Vector2f _enemyPosition = _position + Vector2f(worldSections[Index].tiles[x].position.x * TILESIZE, worldSections[Index].tiles[x].position.y * TILESIZE);
			int _classification = 1;
			enemySpawns.push_back(EnemySpawn(_enemyPosition, _classification));
		}
		if (worldSections[Index].tiles[x].colour == Color(255, 128, 0, 255))
		{
			Vector2f _enemyPosition = _position + Vector2f(worldSections[Index].tiles[x].position.x * TILESIZE, worldSections[Index].tiles[x].position.y * TILESIZE);
			int _classification = 2;
			enemySpawns.push_back(EnemySpawn(_enemyPosition, _classification));
		}
		if (worldSections[Index].tiles[x].colour == Color::Black)
		{
			Vector2f _startPosition, _endPosition;

			_startPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) - (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) - (TILESIZE / 2));
			_endPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) + (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) + (TILESIZE / 2));

			b2EdgeShape* _shape = new b2EdgeShape();
			_shape->Set(Utility::SFVECtoB2VEC(_startPosition, true), Utility::SFVECtoB2VEC(_endPosition, true));

			_fixtureDef = new b2FixtureDef();
			_fixtureDef->shape = _shape;
			_fixtureDef->density = DENSITY;
			_fixtureDef->friction = FRICTION;

			_body->CreateFixture(_fixtureDef);
		}
		if (worldSections[Index].tiles[x].colour == Color(128, 128, 128, 255))
		{
			Vector2f _startPosition, _endPosition;

			_startPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) + (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) - (TILESIZE / 2));
			_endPosition = Vector2f((worldSections[Index].tiles[x].position.x * TILESIZE) - (TILESIZE / 2), (worldSections[Index].tiles[x].position.y * TILESIZE) + (TILESIZE / 2));

			b2EdgeShape* _shape = new b2EdgeShape();
			_shape->Set(Utility::SFVECtoB2VEC(_startPosition, true), Utility::SFVECtoB2VEC(_endPosition, true));

			_fixtureDef = new b2FixtureDef();
			_fixtureDef->shape = _shape;
			_fixtureDef->density = DENSITY;
			_fixtureDef->friction = FRICTION;

			_body->CreateFixture(_fixtureDef);
		}
	}

	_body->SetUserData((void*)ut::WORLD);
	bodyBag.push_back(_body);

	spawnCount++;
}

void WorldGenerator::Generate(RenderWindow &Window, b2World &World, float SpawnPoint)
{
	if(GetLastSpawnPoint() < (SpawnPoint))
	{
		if (menuGeneration)
		{
			SpawnSection(1, World);
		}
		else
		{
			int i = rand() % (/*sectionCount */ 9);
			i += 2;

			SpawnSection(7, World);
			SpawnSection(1, World);
		}
	}
}

void WorldGenerator::Degenerate(Vector2f Cutoff)
{
	for (int i = 0; i < bodyBag.size(); i++)
	{
		if (bodyBag[i]->GetPosition().x < Utility::ScaleToB2(Cutoff.x - backSectionTexture.getSize().x))
		{
			bodyBag[i]->SetUserData((void*)ut::KILL);
		}
	}

	for (int i = 0; i < spawnedSections.size(); i++)
	{
		if (spawnedSections[i].position.x < Cutoff.x - backSectionTexture.getSize().x)
		{
			spawnedSections[i].~SpawnedSection();
			spawnedSections.erase(spawnedSections.begin() + i);
		}
	}
}

void WorldGenerator::PopulateDead(vector<b2Body*>& Vector)
{
	for (int i = 0; i < bodyBag.size(); i++)
	{
		if (bodyBag[i]->GetUserData() == ut::KILL)
		{
			Vector.push_back(bodyBag[i]);
			bodyBag.erase(bodyBag.begin() + i);

			if (bodyBag.size() == 0)
				break;
		}
	}
}

void WorldGenerator::Draw(RenderWindow & Window)
{
	for (int x = 0; x < spawnedSections.size(); x++)
	{
		Window.draw(*spawnedSections[x].backgroundSprite);

		for (int y = 0; y < spawnedSections[x].worldSection.tiles.size(); y++)
		{
			Window.draw(spawnedSections[x].worldSection.tiles[y].sprite);
		}
	}
}

void WorldGenerator::Update()
{
	for (int i = 0; i < enemySpawns.size(); i++)
	{
		if (enemySpawns[i].spawned)
		{
			enemySpawns[i].~EnemySpawn();
			enemySpawns.erase(enemySpawns.begin() + i);
			if (enemySpawns.size() == 0)
				break;
		}
	}

	for (int i = 0; i < destructableSpawns.size(); i++)
	{
		if (destructableSpawns[i].spawned)
		{
			destructableSpawns[i].~DestructableSpawn();
			destructableSpawns.erase(destructableSpawns.begin() + i);
			if (destructableSpawns.size() == 0)
				break;
		}
	}

	for (int x = 0; x < spawnedSections.size(); x++)
	{
		for (int y = 0; y < spawnedSections[x].worldSection.tiles.size(); y++)
		{
			if (spawnedSections[x].worldSection.tiles[y].colour == Color::White || spawnedSections[x].worldSection.tiles[y].colour == Color::Black || spawnedSections[x].worldSection.tiles[y].colour == Color(128, 128, 128, 255))
			{
				Vector2f _position = Vector2f((spawnedSections[x].worldSection.tiles[y].position.x * TILESIZE) + spawnedSections[x].position.x, (spawnedSections[x].worldSection.tiles[y].position.y * TILESIZE));
				
				spawnedSections[x].worldSection.tiles[y].sprite.setOrigin(TILEIMAGESIZE / 2, TILEIMAGESIZE / 2);
				spawnedSections[x].worldSection.tiles[y].sprite.setPosition(_position);
			}
		}
	}
}

void WorldGenerator::GenerateStart(b2World &World)
{
	SpawnSection(1, World);
	SpawnSection(0, World);
	SpawnSection(1, World);
	SpawnSection(1, World);
}

void WorldGenerator::ClearWorld()
{
	int j = spawnedSections.size();

	for (int i = 0; i < j; i++)
	{
		spawnedSections[0].~SpawnedSection();
		spawnedSections.erase(spawnedSections.begin());

		if (spawnedSections.size() == 0)
			break;
	}
}

WorldGenerator::WorldGenerator()
{
}

WorldGenerator::WorldGenerator(string Filepath, b2World &World)
{
	texture.loadFromFile("Images/Bricks.png");
	backSectionTexture.loadFromFile("Images/BackSection.png");
	CreateSectionsFromImage(Filepath);
	GenerateStart(World);
}


WorldGenerator::~WorldGenerator()
{
}
