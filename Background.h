#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "ImageHandler.h"
#include "Utility.h"

#define MOUNTAINSPEED 100.0f
#define MOUNTAINSTRING "Images/Mountains"
#define MOUNTAINSPAWNHEIGHT 192
#define MOUNTAINSPAWNVARIANT 16

class Background
{
private:
	ImageHandler* background;
	vector<ImageHandler*> mountains;
	vector<Texture> mountainTextures;

	int spawnPosition, backgroundID;
public:
	void Update(Vector2f CameraPosition, Vector2f CameraSize, Vector2f PlayerSpeed);
	void Draw(RenderWindow& Window);

	void SpawnObjects(string FilePath, Vector2f Position = Vector2f(0, 0));
	void SpawnObjects(Vector2f Position = Vector2f(0, 0));

	Background(string FilePath, int Id = 0);
	Background();
	~Background();
};

#endif