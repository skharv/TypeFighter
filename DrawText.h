#ifndef DrawText_H
#define DrawText_H

#include "SFML\Graphics.hpp"
#include <string>

#define DEFAULTTEXTSIZE 8

using namespace std;
using namespace sf;

class DrawText
{
private:
	Texture texture;
	Color colour;
	vector<Sprite> sprites;
	Vector2f position, origin;
	Vector2i frames;
	int textSize;
	int length;

	int MapLetter(char Letter);
public:
	void SetLength(int Length) { length = Length; };
	void SetOrigin(Vector2f Origin);
	void SetPosition(Vector2f Position);
	void SetSprites(string* String);
	void SetColour(Color Colour);
	void Draw(RenderWindow &Window);
	void ColourAmount(Color Colour, int Amount);
	void ColourAll(Color Colour);

	Color GetColour() {	return colour; };
	Vector2f GetSize() { return Vector2f(textSize, textSize); };
	Vector2f GetLength() { return Vector2f(length * textSize, textSize); };
	Vector2f GetPosition() { return Vector2f(position.x, position.y); };

	DrawText();
	DrawText(Texture &Texture);
	DrawText(Texture &Texture, int TextSize);
	DrawText(string FilePath, int TextSize);
	~DrawText();
};

#endif
