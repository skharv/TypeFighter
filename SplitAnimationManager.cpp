#include "SplitAnimationManager.h"

void SplitAnimationManager::Update(Vector2f Position)
{
	leader.animHandler->Update(Position);

	for (int i = 0; i < followers.size(); i++)
	{
		if (followers[i].animHandler->GetCurrentSequenceSync())
			followers[i].animHandler->SetCurrentFrame(leader.animHandler->GetCurrentFrame() % followers[i].animHandler->GetCurrentSequenceLength());

		followers[i].animHandler->Update(Position + leader.animHandler->GetCurrentFrameOffset() + followers[i].offset);
	}
}

void SplitAnimationManager::Draw(RenderWindow & Window)
{
	leader.animHandler->Draw(Window);

	for (int i = 0; i < followers.size(); i++)
	{
		followers[i].animHandler->Draw(Window);
	}
}

void SplitAnimationManager::LoadLeader(string Filepath, Vector2i Frames)
{
	Animation _leader = Animation(AnimationHandler(Filepath, Frames), Vector2f(0, 0));

	leader = _leader;
}

void SplitAnimationManager::LoadFollower(string Filepath, Vector2i Frames, Vector2f Offset)
{
	Animation _follower = Animation(AnimationHandler(Filepath, Frames), Offset);

	followers.push_back(_follower);
}

SplitAnimationManager::SplitAnimationManager()
{
}


SplitAnimationManager::~SplitAnimationManager()
{
}
