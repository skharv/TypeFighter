#include "DrawText.h"

int DrawText::MapLetter(char Letter)
{
	int _return = 0;
	switch (Letter)
	{
	case'A':
		_return = 0;
		break;
	case'B':
		_return = 1;
		break;
	case'C':
		_return = 2;
		break;
	case'D':
		_return = 3;
		break;
	case'E':
		_return = 4;
		break;
	case'F':
		_return = 5;
		break;
	case'G':
		_return = 6;
		break;
	case'H':
		_return = 7;
		break;
	case'I':
		_return = 8;
		break;
	case'J':
		_return = 9;
		break;
	case'K':
		_return = 10;
		break;
	case'L':
		_return = 11;
		break;
	case'M':
		_return = 12;
		break;
	case'N':
		_return = 13;
		break;
	case'O':
		_return = 14;
		break;
	case'P':
		_return = 15;
		break;
	case'Q':
		_return = 16;
		break;
	case'R':
		_return = 17;
		break;
	case'S':
		_return = 18;
		break;
	case'T':
		_return = 19;
		break;
	case'U':
		_return = 20;
		break;
	case'V':
		_return = 21;
		break;
	case'W':
		_return = 22;
		break;
	case'X':
		_return = 23;
		break;
	case'Y':
		_return = 24;
		break;
	case'Z':
		_return = 25;
		break;
	case' ':
		_return = 26;
		break;
	case'.':
		_return = 27;
		break;
	case':':
		_return = 28;
		break;
	case'!':
		_return = 29;
		break;
	case'1':
		_return = 30;
		break;
	case'2':
		_return = 31;
		break;
	case'3':
		_return = 32;
		break;
	case'4':
		_return = 33;
		break;
	case'5':
		_return = 34;
		break;
	case'6':
		_return = 35;
		break;
	case'7':
		_return = 36;
		break;
	case'8':
		_return = 37;
		break;
	case'9':
		_return = 38;
		break;
	case'0':
		_return = 39;
		break;
	case'-':
		_return = 40;
		break;
	case'?':
		_return = 41;
		break;
	default:
		_return = 41;
		break;
	}

	return _return;
}

void DrawText::SetOrigin(Vector2f Origin)
{
	origin = Origin;
	for (int i = 0; i < sprites.size(); i++)
		sprites[i].setOrigin(origin);
}

void DrawText::SetPosition(Vector2f Position)
{
	position = Position;
	for (int i = 0; i < sprites.size(); i++)
		sprites[i].setPosition(position.x + (i*textSize), position.y);
}

void DrawText::SetSprites(string * String)
{
	sprites.clear();
	length = String->size();

	for (int i = 0; i < String->size(); i++)
	{
		int _index = MapLetter(String->at(i));
		sprites.push_back(Sprite(texture, IntRect((_index % frames.x) * textSize, (_index / frames.x) * textSize, textSize, textSize)));
	}
}

void DrawText::SetColour(Color Colour)
{
	colour = Colour;

	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i].setColor(colour);
	}
}

void DrawText::Draw(RenderWindow &Window)
{ 
	for (int i = 0; i < sprites.size(); i++)
	{
		Window.draw(sprites[i]); // why does this return to 0,0?
	}
}

void DrawText::ColourAmount(Color Colour, int Amount)
{
	for (int i = 0; i < Amount; i++)
	{
		if (i < length)
			sprites[i].setColor(Colour);
	}
}

void DrawText::ColourAll(Color Colour)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i].setColor(Colour);
	}
}

DrawText::DrawText()
{
	texture.loadFromFile("Images/TestText.png");
	textSize = DEFAULTTEXTSIZE;
	frames = Vector2i(texture.getSize().x / textSize, texture.getSize().y / textSize);
	colour = Color::White;
}

DrawText::DrawText(Texture &Texture)
{
	texture = Texture;
	textSize = DEFAULTTEXTSIZE;
	frames = Vector2i(texture.getSize().x / textSize, texture.getSize().y / textSize);
	colour = Color::White;
}

DrawText::DrawText(Texture & Texture, int TextSize)
{
	texture = Texture;
	textSize = TextSize;
	frames = Vector2i(texture.getSize().x / textSize, texture.getSize().y / textSize);
	colour = Color::White;
}

DrawText::DrawText(string FilePath, int TextSize)
{
	texture.loadFromFile(FilePath);
	textSize = TextSize;
	frames = Vector2i(texture.getSize().x / textSize, texture.getSize().y / textSize);
	colour = Color::White;
}

DrawText::~DrawText()
{
}
