#include "DistanceBoard.h"

void DistanceBoard::Update()
{
	if (onTheLine)
	{
		SteamAPI_RunCallbacks();

		if (steamLeaderboard->IsBoardReady())
			steamLeaderboard->DownloadTopScore();
	}
	
	if (alpha != leaderboardHeading->GetColour().a)
	{
		leaderboardHeading->SetColour(Color(leaderboardHeading->GetColour().r, leaderboardHeading->GetColour().g, leaderboardHeading->GetColour().b, alpha));

		for (int i = 0; i < entries; i++)
		{
			leaderboardEntry[i]->SetColour(Color(leaderboardEntry[i]->GetColour().r, leaderboardEntry[i]->GetColour().g, leaderboardEntry[i]->GetColour().b, alpha));
		}
	}
}

void DistanceBoard::Draw(RenderWindow &Window)
{
	leaderboardHeading->Draw(Window);
	if (onTheLine)
		for (int i = 0; i < LEADERBOARDSIZE; i++)
			leaderboardEntry[i]->Draw(Window);
}

void DistanceBoard::UploadDistance(int Distance)
{
	if (onTheLine)
	{
		steamLeaderboard->UploadScore(Distance);
		

		steamLeaderboard->DownloadScore();
	}
}

void DistanceBoard::UpdateBoard(Vector2f ScreenPosition)
{
	leaderboardHeading->SetPosition(Vector2f(position.x + ScreenPosition.x, position.y + ScreenPosition.y));

	if (onTheLine)
		steamLeaderboard->leaderboardEntriesSize < LEADERBOARDSIZE ? entries = steamLeaderboard->leaderboardEntriesSize : entries = LEADERBOARDSIZE;

	switch (align)
	{
	case Left:
		leaderboardHeading->SetOrigin(Vector2f(0, (leaderboardHeading->GetLength().y / 2)));
		break;
	case Center:
		leaderboardHeading->SetOrigin(Vector2f((leaderboardHeading->GetLength().x / 2), (leaderboardHeading->GetLength().y / 2)));
		break;
	case Right:
		leaderboardHeading->SetOrigin(Vector2f((leaderboardHeading->GetLength().x), (leaderboardHeading->GetLength().y / 2)));
		break;
	default:
		leaderboardHeading->SetOrigin(Vector2f((leaderboardHeading->GetLength().x), (leaderboardHeading->GetLength().y / 2)));
		break;
	}
	for (int i = 0; i < entries; i++)
	{
		if (onTheLine)
		{
			string s;
			s = to_string(steamLeaderboard->leaderboardEntries[i].m_nGlobalRank);
			s += ".";
			s += SteamFriends()->GetFriendPersonaName(steamLeaderboard->leaderboardEntries[i].m_steamIDUser);
			s += " - ";
			s += to_string(steamLeaderboard->leaderboardEntries[i].m_nScore);

			for (int j = 0; j < s.length(); j++)
			{
				if (islower(s[j]))
					s[j] = toupper(s[j]);
			}

			leaderboardEntry[i]->SetSprites(&s);

			string a = SteamFriends()->GetFriendPersonaName(steamLeaderboard->leaderboardEntries[i].m_steamIDUser);
			string b = SteamFriends()->GetPersonaName();

			if (a == b)
				leaderboardEntry[i]->SetColour(Color::Green);
			else
				leaderboardEntry[i]->SetColour(Color::White);
		}
		switch (align)
		{
		case Left:leaderboardEntry[i]->SetOrigin(Vector2f(0, (leaderboardEntry[i]->GetLength().y / 2)));
			break;
		case Center:leaderboardEntry[i]->SetOrigin(Vector2f((leaderboardEntry[i]->GetLength().x / 2), (leaderboardEntry[i]->GetLength().y / 2)));
			break;
		case Right:leaderboardEntry[i]->SetOrigin(Vector2f((leaderboardEntry[i]->GetLength().x), (leaderboardEntry[i]->GetLength().y / 2)));
			break;
		default:
			leaderboardEntry[i]->SetOrigin(Vector2f(0, (leaderboardEntry[i]->GetLength().y / 2)));
			break;
		}

		leaderboardEntry[i]->SetPosition(Vector2f(leaderboardHeading->GetPosition().x, leaderboardHeading->GetPosition().y + (((leaderboardEntry[i]->GetSize().y + verticalGap)* (i + 1)))));
	}
}

DistanceBoard::DistanceBoard(const char* Name, Vector2f PositionOnScreen, Alignment Align, float VerticalGap)
{
	onTheLine = SteamAPI_Init();

	position = PositionOnScreen;
	verticalGap = VerticalGap;
	alpha = 0;
	align = Align;

	if (onTheLine)
	{
		steamLeaderboard = new LeaderboardHandler();
		steamLeaderboard->FindLeaderboard(Name);

		leaderboardHeading = new DrawText("Images/TestText.png", 8);
		leaderboardHeading->SetSprites(new string("LEADERBOARD:"));

		steamLeaderboard->leaderboardEntriesSize < LEADERBOARDSIZE ? entries = steamLeaderboard->leaderboardEntriesSize : entries = LEADERBOARDSIZE;

		for (int i = 0; i < LEADERBOARDSIZE; i++)
		{
			leaderboardEntry[i] = new DrawText("Images/TestText.png", 8);
			string s = "NULL";

			leaderboardEntry[i]->SetSprites(&s);
			leaderboardEntry[i]->SetColour(Color::Transparent);
		}

	}
	else
	{
		leaderboardHeading = new DrawText("Images/TestText.png", 8);
		leaderboardHeading->SetSprites(new string("LEADERBOARD UNAVAILABLE"));
	}
}

DistanceBoard::DistanceBoard()
{
	leaderboardHeading = new DrawText("Images/TestText.png", 8);
	leaderboardHeading->SetSprites(new string("LEADERBOARD UNAVAILABLE"));

	onTheLine = false;
}


DistanceBoard::~DistanceBoard()
{
}
