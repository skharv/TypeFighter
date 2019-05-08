#include "StatsAchHandler.h"

void StatsAchHandler::RequestStats()
{
	GotStats = SteamStats->RequestCurrentStats();
}

void StatsAchHandler::CompleteAchievement(Achievements achievement)
{
	if (GotStats)
	{
		switch (achievement)
		{
		case ACH_RUN_250:
			SteamStats->SetAchievement("ACH_RUN_250");
			break;
		case ACH_RUN_500:
			SteamStats->SetAchievement("ACH_RUN_500");
			break;
		case ACH_RUN_1000:
			SteamStats->SetAchievement("ACH_RUN_1000");
			break;
		case ACH_RUN_10000:
			SteamStats->SetAchievement("ACH_RUN_10000");
			break;
		case ACH_DIE:
			SteamStats->SetAchievement("ACH_DIE");
			break;
		case ACH_SHOOT_SOMETHING:
			SteamStats->SetAchievement("ACH_SHOOT_SOMETHING");
			break;
		case ACH_NAVIGATE_MENU:
			SteamStats->SetAchievement("ACH_NAVIGATE_MENU");
			break;
		case ACH_SLASH_SOMETHING:
			SteamStats->SetAchievement("ACH_SLASH_SOMETHING");
			break;
		case ACH_STAND_STILL:
			SteamStats->SetAchievement("ACH_STAND_STILL");
			break;
		case ACH_1000_WORDS:
			SteamStats->SetAchievement("ACH_1000_WORDS");
			break;
		default:
			break;
		}
	}
}

void StatsAchHandler::SetBestDistance(int Distance)
{
	if (GotStats)
	{
		int longestRun;
		SteamStats->GetStat("Longest Run", &longestRun);
		if (Distance > longestRun)
			SteamStats->SetStat("Longest Run", Distance);
	}
}

void StatsAchHandler::IncrementWordStat()
{
	if (GotStats)
	{
		int wordsTyped;
		SteamStats->GetStat("Words Typed", &wordsTyped);
		SteamStats->SetStat("Words Typed", wordsTyped + 1);
		if (wordsTyped + 1 >= 1000)
			CompleteAchievement(ACH_1000_WORDS);
	}
}

void StatsAchHandler::IncrementRunsMade()
{
	if (GotStats)
	{
		int runsMade;
		SteamStats->GetStat("Runs Made", &runsMade);
		SteamStats->SetStat("Runs Made", runsMade + 1);
	}
}

StatsAchHandler::StatsAchHandler()
{
	SteamStats = SteamUserStats();
	RequestStats();
}


StatsAchHandler::~StatsAchHandler()
{
}
