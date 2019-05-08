#ifndef STATSACHHANDLER_H
#define STATSACHHANDLER_H

#include <steam\steam_api.h>

enum Achievements
{
	ACH_RUN_250 = 0,
	ACH_RUN_500 = 1,
	ACH_RUN_1000 = 2,
	ACH_RUN_10000 = 3,
	ACH_DIE = 4,
	ACH_SHOOT_SOMETHING = 5,
	ACH_NAVIGATE_MENU = 6,
	ACH_SLASH_SOMETHING = 7,
	ACH_STAND_STILL = 8,
	ACH_1000_WORDS = 9
};

class StatsAchHandler
{
private:
	ISteamUserStats *SteamStats;
	bool GotStats = false;
public:
	void RequestStats();
	void CompleteAchievement(Achievements achievement);

	void SetBestDistance(int Distance);
	void IncrementWordStat();
	void IncrementRunsMade();

	StatsAchHandler();
	~StatsAchHandler();
};

#endif