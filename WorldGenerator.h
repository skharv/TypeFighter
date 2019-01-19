#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <AnimationHandler.h>
#include "Utility.h"
#include "Destructable.h"
#include "NPC.h"

#define SECTIONWIDTH 8
#define TILESIZE 16 
#define TILEIMAGESIZE 16
#define FRICTION 0.1f
#define DENSITY 1.0f

using namespace std;
using namespace sf;

class WorldGenerator
{
private:
	Image image;
	bool menuGeneration = true;
	int sectionCount = 0;
	int spawnCount = 0;
	Vector2f playerSpawn = Vector2f(0, 0);
	Texture texture, backSectionTexture;

	struct TileData
	{
		TileData(Color Colour, Vector2f Position, Vector2f Size) : colour(Colour), position(Position), size(Size)
		{
		};

		TileData(){};

		Color colour;
		Vector2f position;
		Vector2f size;
		Sprite sprite;
		int tileIndex;

		TileData *up, *right, *down, *left;
	};

	struct WorldSection
	{
		vector<TileData> tiles;
	};

	struct EnemySpawn
	{
		EnemySpawn(Vector2f Position, int Classification) : position(Position), classification(Classification) {};
		~EnemySpawn() {};

		bool spawned = false;
		Vector2f position;
		int classification;
	};

	struct DestructableSpawn
	{
		DestructableSpawn(Vector2f Position, int Classification) : position(Position), classification(Classification) {};
		~DestructableSpawn() {};

		bool spawned = false;
		Vector2f position;
		int classification;
	};

	struct SpawnedSection
	{
		~SpawnedSection() {};
		Vector2f position;
		Sprite* backgroundSprite;
		WorldSection worldSection;
	};

	AnimationHandler backSection;

	vector<WorldSection> worldSections;
	vector<SpawnedSection> spawnedSections;
	vector<EnemySpawn> enemySpawns;
	vector<DestructableSpawn> destructableSpawns;
	vector<NPC> enemies;
	vector<b2Body*> bodyBag;

	bool CreateSectionsFromImage(string Filepath);
	void SpawnSection(int Index, b2World &World);
public:
	void Generate(RenderWindow &Window, b2World &World, float SpawnPoint);
	void Degenerate(Vector2f Cutoff);
	void PopulateDead(vector<b2Body*>& Vector);

	void Draw(RenderWindow &Window);
	void Update();

	void SetMenuGeneration(bool value) { menuGeneration = value; };
	void GenerateStart(b2World &World);
	void ClearWorld();

	Vector2f GetPlayerSpawn() { return playerSpawn; };
	vector<EnemySpawn> GetEnemySpawns() { return enemySpawns; };
	vector<DestructableSpawn> GetDestructableSpawns() { return destructableSpawns; };
	void EnemyHasSpawned(int Index) { enemySpawns[Index].spawned = true; };
	void DestructableHasSpawned(int Index) { destructableSpawns[Index].spawned = true; };
	float GetLastSpawnPoint() { return spawnCount * TILESIZE * SECTIONWIDTH; };

	WorldGenerator();
	WorldGenerator(string Filepath, b2World &World);
	~WorldGenerator();
};

#endif
