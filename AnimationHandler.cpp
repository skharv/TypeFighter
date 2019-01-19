#include "AnimationHandler.h"

void AnimationHandler::Draw(RenderWindow & Window)
{
	Window.draw(sprite);
}

void AnimationHandler::Update(Vector2f Position)
{
	if (counter-- < 0)
	{
		counter = frameRate;
		currentIndex++;

		for (int i = 0; i < sequences.size(); i++)
		{
			if (sequences[i].name == currentAnimation)
			{
				if (currentIndex >= sequences[i].frameIndices.size() && sequences[i].loop)
					currentIndex = 0;
				else if (currentIndex >= sequences[i].frameIndices.size() && !sequences[i].loop && sequences[i].stayOnLastFrame)
				{
					currentIndex = sequences[i].frameIndices.size() - 1;
				}
				else if (currentIndex >= sequences[i].frameIndices.size() && !sequences[i].loop && !sequences[i].stayOnLastFrame)
				{
					if(currentAnimation == "Die")
						currentIndex = 0;

					currentAnimation = defaultAnimation;
					counter = frameRate;
					currentIndex = 0;
				}
			}
		}
		for (int i = 0; i < sequences.size(); i++)
		{
			if (sequences[i].name == currentAnimation)
			{
				sprite = Sprite(texture, IntRect((sequences[i].frameIndices.at(currentIndex).index % frames.x) * frameSize.x,
					(sequences[i].frameIndices.at(currentIndex).index / frames.x) * frameSize.y, frameSize.x, frameSize.y));
				sprite.setOrigin(origin);
				sprite.setScale(scale);
			}
		} 
	}
	sprite.setPosition(Position + offset);
}

int AnimationHandler::GetCurrentSequenceLength()
{
	int _return = 0;

	for (int i = 0; i < sequences.size(); i++)
	{
		if (sequences[i].name == currentAnimation)
		{
			_return = sequences[i].frameIndices.size();
		}
	}

	return _return;
}

bool AnimationHandler::GetCurrentSequenceSync()
{
	bool _return = false;

	for (int i = 0; i < sequences.size(); i++)
	{
		if (sequences[i].name == currentAnimation)
		{
			_return = sequences[i].sync;
		}
	}

	return _return;
}

Vector2f AnimationHandler::GetCurrentFrameOffset(int FrameSync)
{	
	Vector2f _return = Vector2f(0, 0);

	for (int i = 0; i < sequences.size(); i++)
	{
		if (sequences[i].name == currentAnimation)
		{
			_return = sequences[i].frameIndices.at(currentIndex < 0?0: currentIndex).offset;
		}
	}

	return _return;
}

void AnimationHandler::SetCurrentAnimation(string Name)
{
	currentAnimation = Name;
	currentIndex = 0;
	counter = frameRate;

	for (int i = 0; i < sequences.size(); i++)
	{
		if (sequences[i].name == currentAnimation)
		{
			sprite = Sprite(texture, IntRect((sequences[i].frameIndices.at(currentIndex).index % frames.x) * frameSize.x,
				(sequences[i].frameIndices.at(currentIndex).index / frames.x) * frameSize.y, frameSize.x, frameSize.y));
			sprite.setOrigin(origin);
			sprite.setScale(scale);
		}
	}
}

void AnimationHandler::CreateAnimation(string Name, vector<frameIndex> Frames, bool Loop, bool StayOnLastFrame, bool Sync)
{
	Sequence _sequence(Name, Frames, Loop, StayOnLastFrame, Sync);
	sequences.push_back(_sequence);
}

void AnimationHandler::CreateAnimation(string Name, vector<int> Frames, bool Loop, bool StayOnLastFrame, bool Sync)
{
	vector<frameIndex> _frames;

	for (int i = 0; i < Frames.size(); i++)
	{
		_frames.push_back(frameIndex(Frames[i]));
	}

	Sequence _sequence(Name, _frames, Loop, StayOnLastFrame, Sync);
	sequences.push_back(_sequence);
}

AnimationHandler::AnimationHandler()
{
}

AnimationHandler::AnimationHandler(string Filepath, Vector2i Frames)
{
	texture.loadFromFile(Filepath);
	frames = Frames;
	frameSize = Vector2i(texture.getSize().x / frames.x, texture.getSize().y / frames.y);
	scale = Vector2f(1, 1);
	origin = Vector2f(frameSize.x / 2, frameSize.y / 2);
	offset = Vector2f(0, 0);
	frameRate = DEFAULTFRAMERATE;
	currentIndex = DEFAULTFRAMEINDEX;
}

AnimationHandler::~AnimationHandler()
{
}
