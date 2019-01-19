#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <SFML\Graphics.hpp>

using namespace std;
using namespace sf;

class ImageHandler
{
private:
	Texture texture;
	Sprite sprite;
	Vector2f origin;
	Vector2f scale;
public:
	void Draw(RenderWindow& Window);
	void Update(Vector2f Position);

	void SetPosition(Vector2f Position) { sprite.setPosition(Position); };
	void SetOrigin(Vector2f Origin) { origin = Origin; };
	void SetRotation(float Degrees) { sprite.setRotation(Degrees); };

	Vector2f GetPosition() { return sprite.getPosition(); };
	Vector2u GetSize() { return sprite.getTexture()->getSize(); };

	ImageHandler(string FilePath);
	ImageHandler(Texture LoadTexture);
	ImageHandler();
	~ImageHandler();
};

#endif