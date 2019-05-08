#include "ImageHandler.h"

ImageHandler::ImageHandler()
{
}

void ImageHandler::Draw(RenderWindow & Window)
{
	Window.draw(sprite);
}

void ImageHandler::Update(Vector2f Position)
{
	sprite.setPosition(Position + offset);
}

ImageHandler::ImageHandler(string FilePath)
{
	texture.loadFromFile(FilePath);
	scale = Vector2f(1, 1);
	offset = Vector2f(0, 0);
	origin = Vector2f(texture.getSize().x / 2, texture.getSize().y / 2);
	sprite = Sprite(texture);
	sprite.setOrigin(origin);
}

ImageHandler::ImageHandler(Texture LoadTexture)
{
	texture = LoadTexture;
	scale = Vector2f(1, 1);
	offset = Vector2f(0, 0);
	origin = Vector2f(texture.getSize().x / 2, texture.getSize().y / 2);
	sprite = Sprite(texture);
	sprite.setOrigin(origin);
}


ImageHandler::~ImageHandler()
{
}
