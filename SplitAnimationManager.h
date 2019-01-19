#ifndef SPLITANIMATIONHANDLER_H
#define SPLITANIMATIONHANDLER_H

#include "AnimationHandler.h"

class SplitAnimationManager
{
private:

	struct Animation
	{
		Animation() {};
		Animation(AnimationHandler AnimHandler, Vector2f Offset) { animHandler = new AnimationHandler(AnimHandler); offset = Offset; };
		AnimationHandler* animHandler;
		Vector2f offset;
	};

	Animation leader;
	vector<Animation> followers;
public:
	void Update(Vector2f Position);
	void Draw(RenderWindow& Window);

	Animation GetLeader() { return leader; };
	Animation GetFollower(int Index) { return followers[Index]; };
	int GetFollowerSize() { return followers.size(); };

	void LoadLeader(string Filepath, Vector2i Frames);
	void LoadFollower(string Filepath, Vector2i Frames, Vector2f Offset);
	void SetAnimation(string AnimationName) { leader.animHandler->SetCurrentAnimation(AnimationName); followers[0].animHandler->SetCurrentAnimation(AnimationName);	};

	SplitAnimationManager();
	~SplitAnimationManager();
};

#endif SPLITANIMATIONHANDLER_H
