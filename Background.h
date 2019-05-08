#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "ImageHandler.h"
#include "Utility.h"

#define MOUNTAINSPEED 1
#define MOUNTAINSTRING "Images/Mountains"
#define MOUNTAINSPAWNHEIGHT 192
#define MOUNTAINSPAWNVARIANT 16

#define CLOUDSPEEDMULTIPLIER 1.5
#define CLOUDSTRING "Images/Cloud"
#define CLOUDSPAWNHEIGHT 0
#define CLOUDSPAWNVARIANT 200

class Background
{
private:
	ImageHandler* background;
	vector<ImageHandler*> mountains;
	vector<Texture> mountainTextures;
	vector<ImageHandler*> clouds;
	vector<Texture> cloudTextures;

	int spawnPosition, backgroundID, cloudDensity;
public:
	void Update(Vector2f CameraPosition, Vector2f CameraSize, Vector2f PlayerSpeed);
	void Draw(RenderWindow& Window);

	void SpawnMountain(string FilePath, Vector2f Position = Vector2f(0, 0));
	void SpawnMountain(Vector2f Position = Vector2f(0, 0));
	void SpawnCloud(string FilePath, Vector2f Position = Vector2f(0, 0));
	void SpawnCloud(Vector2f Position = Vector2f(0, 0));

	Background(string FilePath, int Id = 0);
	Background(string FilePath, Vector2f ScreenPosition, int Id = 0);
	Background();
	~Background();
};

#endif