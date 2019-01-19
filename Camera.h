#ifndef CAMERA_H
#define CAMERA_H

#include "SFML\Graphics.hpp"

using namespace sf;

class Camera
{
private:
	Vector2f position, vector, size, windowSize;

	bool fade = false;

	View view;
	RectangleShape viewWindow;
public:
	View GetCamera() { return view; };
	Vector2f GetPosition() { return position; };
	Vector2f GetSize() { return size; };
	Vector2f GetStartPoint() { return Vector2f(view.getCenter().x - (view.getSize().x / 2), view.getCenter().y - (view.getSize().y / 2)); };
	Vector2f GetDespawnPoint() { return Vector2f(view.getCenter().x - view.getSize().x, view.getCenter().y + (view.getSize().y / 2)); };
	RectangleShape GetViewWindow() { return viewWindow; };
	void SetZoom(float Zoom) { view.zoom(Zoom); };

	void Resize(Vector2u NewSize);

	void ApplyVector(Vector2f Vector) { vector += Vector; };
	void SetPosition(Vector2f Position) { position = Position; };

	void Update(RenderWindow &Window);

	Camera(Vector2f Size);
	Camera();
	~Camera();
};

#endif

