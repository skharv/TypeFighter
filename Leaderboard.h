#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <steam\steam_api.h>

class Leaderboard
{
private:
	SteamLeaderboard_t currentLeaderboard;

	int min(int a, int b) { return (a < b ? a : b); };
public:
	int leaderboardEntriesSize;
	LeaderboardEntry_t leaderboardEntries[10];

	void FindLeaderboard(const char *LeaderboardName);
	bool UploadScore( int Score);
	bool DownloadScore();


	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<Leaderboard, LeaderboardFindResult_t> callResultFindLeaderboard;
	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult<Leaderboard, LeaderboardScoreUploaded_t> callResultUploadScore;
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult<Leaderboard, LeaderboardScoresDownloaded_t> callResultDownloadScore;

	Leaderboard();
	~Leaderboard();
};

#endif

