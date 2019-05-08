#include "Camera.h"

void Camera::Resize(Vector2u NewSize)
{
	Vector2f sizePercentage;

	size = Vector2f(NewSize.x/* * 0.25*/, NewSize.y/* * 0.25*/);
	view.reset(FloatRect(0, 0, size.x, size.y));
}

void Camera::Update(RenderWindow & Window)
{
	size = view.getSize();
	position += vector;
	view.setCenter(position);

	Window.setView(view);
}

Camera::Camera(Vector2f Size)
{
	size = Size;
	position = Vector2f(0, 0);
	view.reset(FloatRect(0, 0, size.x, size.y));
	view.setCenter(Vector2f(position));
}

Camera::Camera()
{
}


Camera::~Camera()
{
}
