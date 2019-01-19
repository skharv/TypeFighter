#ifndef ANIMATIONHANDLER_H
#define ANIMATIONHANDLER_H

#include <SFML\Graphics.hpp>

#define DEFAULTFRAMERATE 5
#define DEFAULTFRAMEINDEX 0

using namespace std;
using namespace sf;

class AnimationHandler
{
private:
	Texture texture;
	Sprite sprite;
	Vector2i frames;
	Vector2i frameSize;
	Vector2f origin;
	Vector2f offset;
	Vector2f scale;
	bool loop = true;
	int counter = 0;
	int frameRate;
	int currentIndex;
	string currentAnimation;
	string defaultAnimation;

	struct frameIndex;

	struct Sequence
	{
		Sequence() {};
		Sequence(String Name, vector<frameIndex> FrameIndices, bool Loop, bool StayOnLastFrame, bool Sync = false)
		{
			name = Name;
			frameIndices = FrameIndices;
			loop = Loop;
			stayOnLastFrame = StayOnLastFrame;
			if (loop)
				stayOnLastFrame = false;
			sync = Sync;
		};
		string name;
		vector<frameIndex> frameIndices;
		bool loop;
		bool sync;
		bool stayOnLastFrame;
	};

	vector<Sequence> sequences;
	//map<string, vector<int>> sequences; 

public:
	struct frameIndex
	{
		frameIndex() {};
		frameIndex(int Index, Vector2f Offset = Vector2f(0, 0)) { index = Index, offset = Offset; };

		int index;
		Vector2f offset;
	};

	void Draw(RenderWindow& Window);
	void Update(Vector2f Position);
	string GetCurrentAnimation() { return currentAnimation; };
	int GetCurrentSequenceLength();
	bool GetCurrentSequenceSync();
	Vector2f GetCurrentFrameOffset(int FrameSync = 0);
	int GetCurrentFrame() { return currentIndex; };

	void SetPosition(Vector2f Position) { sprite.setPosition(Position); };
	void SetFrameRate(int FrameRate) { frameRate = FrameRate; };
	void SetCurrentFrame(int FrameIndex) { currentIndex = FrameIndex; };
	void SetOrigin(Vector2f Origin) { origin = Origin; };
	void SetOffset(Vector2f Offset) { offset = Offset; };
	void SetRotation(float Degrees) { sprite.setRotation(Degrees); };
	void SetCurrentAnimation(string Name);
	void SetDefaultAnimation(string Name) { defaultAnimation = Name; currentAnimation = Name; };
	void CreateAnimation(string Name, vector<frameIndex> Frames, bool Loop, bool StayOnLastFrame, bool Sync = false);
	void CreateAnimation(string Name, vector<int> Frames, bool Loop, bool StayOnLastFrame, bool Sync = false);
	void SetLoop(bool Loop) { loop = Loop; };

	AnimationHandler();
	AnimationHandler(string FilePath, Vector2i Frames);
	~AnimationHandler();
};

#endif